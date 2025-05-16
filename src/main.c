#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

typedef struct{
    uint32_t status;
    uint32_t ctrl; 
}GPIOregs;

#define GPIO ((GPIOregs*)GPIOBase)

typedef struct
{
    uint32_t Out;
    uint32_t OE;
    uint32_t In;
    uint32_t InSync;
} rioregs;

#define rio ((rioregs *)RIOBase)
#define rioXOR ((rioregs *)(RIOBase + 0x1000 / 4))
#define rioSET ((rioregs *)(RIOBase + 0x2000 / 4))
#define rioCLR ((rioregs *)(RIOBase + 0x3000 / 4))


void init_panel_pins(GPIOregs* gpio, uint32_t* pad, rioregs* rio_regs) 
{
    // RGB Pins
    int rgb_pins[] = { 5, 13, 6, 12, 16, 23 }; // R1, G1, B1, R2, G2, B2
    // Address Pins A-E
    int addr_pins[] = { 22, 26, 27, 20, 8 };
    // Control Pins
    int ctrl_pins[] = { 4, 17, 21 }; // OE, CLK, LAT

    int all_pins[sizeof(rgb_pins)/4 + sizeof(addr_pins)/4 + sizeof(ctrl_pins)/4];
    int pin_count = 0;

    for (int i = 0; i < sizeof(rgb_pins)/4; i++) all_pins[pin_count++] = rgb_pins[i];
    for (int i = 0; i < sizeof(addr_pins)/4; i++) all_pins[pin_count++] = addr_pins[i];
    for (int i = 0; i < sizeof(ctrl_pins)/4; i++) all_pins[pin_count++] = ctrl_pins[i];

    for (int i = 0; i < pin_count; i++) {
        int pin = all_pins[i];
        gpio[pin].ctrl = 5;         // 5 = SIO (software-controlled output)
        pad[pin + 1] = 0x10;        // Strong drive, no pull, default
        rio_regs->OE = 0x01 << pin;      // Output Enable setzen
        rio_regs->Out = 0x01 << pin;
    }
}

void draw_full_blue_frame(rioregs* rio_set, rioregs* rio_clr) {
    int R1 = 5, G1 = 13, B1 = 6, R2 = 12, G2 = 16, B2 = 23; 
    int CLK = 17, LAT = 21, OE = 4;
    int A = 22, B = 26, C = 27, D = 20, E = 8;

    for (int row = 0; row < 24; row++) {
        // Adressbits setzen (A–E)
        if (row & 1) rio_set->Out = (1 << A); else rio_clr->Out = (1 << A);
        if (row & 2) rio_set->Out = (1 << B); else rio_clr->Out = (1 << B);
        if (row & 4) rio_set->Out = (1 << C); else rio_clr->Out = (1 << C);
        if (row & 8) rio_set->Out = (1 << D); else rio_clr->Out = (1 << D);
        if (row & 16) rio_set->Out = (1 << E); else rio_clr->Out = (1 << E);

        // 96 Pixel blau schieben
        for (int i = 0; i < 96; i++) {
            rio_set->Out = (1 << B1);
            rio_clr->Out = (1 << R1) | (1 << G1);

            rio_set->Out = (1 << B2);
            rio_clr->Out = (1 << R2) | (1 << G2);

            rio_set->Out = (1 << CLK);
            rio_clr->Out = (1 << CLK);
        }

        // Latch
        rio_set->Out = (1 << LAT);
        rio_clr->Out = (1 << LAT);

        // OE aktivieren
        rio_clr->Out = (1 << OE);
        usleep(100); // Anzeigezeit
        rio_set->Out = (1 << OE);
    }
}

#include <time.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int memfd = open("/dev/mem", O_RDWR | O_SYNC);
    uint32_t *map = (uint32_t *)mmap(
        NULL,
        64 * 1024 * 1024,
        (PROT_READ | PROT_WRITE),
        MAP_SHARED,
        memfd,
        0x1f00000000
    );
    if (map == MAP_FAILED)
    {
        printf("mmap failed: %s\n", strerror(errno));
        return (-1);
    };
    close(memfd);

    uint32_t *PERIBase = map;
    uint32_t *GPIOBase = PERIBase + 0xD0000 / 4;
    uint32_t *RIOBase = PERIBase + 0xe0000 / 4;
    uint32_t *PADBase = PERIBase + 0xf0000 / 4;
    uint32_t *pad = PADBase + 1;   
    init_panel_pins(GPIO, pad, rioSET);

    #if false
    uint32_t pin = 2;
    uint32_t fn = 5;
    GPIO[pin].ctrl = fn;
    pad[pin] = 0x10;

    rioSET->OE = 0x01<<pin;
    rioSET->Out = 0x01<<pin;
    #endif

    struct timespec t_start, t_now;
    int frame_count = 0;

    clock_gettime(1, &t_start);

    for (;;) 
    {
        draw_full_blue_frame(rioSET, rioCLR);
        frame_count++;

        clock_gettime(1, &t_now);
        double elapsed =
            (t_now.tv_sec - t_start.tv_sec) +
            (t_now.tv_nsec - t_start.tv_nsec) / 1e9;

        if (elapsed >= 1.0) 
        {
            printf("FPS: %d\n", frame_count);
            frame_count = 0;
            t_start = t_now;
        }
    }

    return (EXIT_SUCCESS);
} 
