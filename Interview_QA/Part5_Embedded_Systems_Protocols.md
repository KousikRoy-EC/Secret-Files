# Part 5: Embedded Systems, Protocols, and Hardware

---

## Q1: How I2C Works (Master-Slave Communication)

I2C (Inter-Integrated Circuit) is a synchronous, multi-master, multi-slave, half-duplex, 2-wire serial bus. The two lines are:
- **SDA (Serial Data Line):** Bidirectional line for data transfer.
- **SCL (Serial Clock Line):** Clock line driven by the master.

Both lines are open-drain, meaning they must be pulled up to \(V_{CC}\) via external pull-up resistors.

```
       VCC
        │   [Pull-up Resistors]
        ├──[R]──┐
        ├──[R]──┼──┐
        │       │  │
    ┌───┴───┐   │  │   ┌───────┐
    │Master │───┼──┼──>│Slave 1│
    │       │   │  │   └───────┘
    │  SCL  │───┘  │   ┌───────┘
    │  SDA  │──────┘──>│Slave 2│
    └───────┘          └───────┘
```

### Communication Steps
1. **Idle State:** Both SDA and SCL are pulled High.
2. **Start Condition (S):** Master pulls SDA Low while SCL is High.
3. **Address Frame:** Master transmits 7-bit slave address + 1 Read/Write bit (0 for Write, 1 for Read).
4. **ACK/NACK:** The slave with the matching address pulls SDA Low (ACK) during the 9th clock pulse.
5. **Data Frame:** Master or Slave transmits 8-bit data.
6. **ACK/NACK:** Receiver pulls SDA Low to acknowledge the byte.
7. **Stop Condition (P):** Master transitions SDA from Low to High while SCL is High.

---

## Q2: SCL State during SDA Start Bit

### Answer
At the start of an I2C transaction, the **Start Condition** is defined as a High-to-Low transition on the SDA line while SCL is **High**. Therefore, SCL must be **High** when the Start bit begins.

```
SCL:  ───────┐
             └──────
SDA:  ────┐
          └─────────
          ^ START Bit (SDA falls while SCL is High)
```

---

## Q3: Is I2C or SPI Asynchronous?

### Answer
**Neither.** Both I2C and SPI are **synchronous** protocols because they both utilize a dedicated clock line (SCL in I2C, SCK in SPI) to synchronize data transfer. 
- **UART** is the asynchronous protocol among common serial protocols (it has no clock line and relies on pre-configured baud rates).

---

## Q4: SPI Protocol CPOL and CPHA Modes

SPI (Serial Peripheral Interface) is a synchronous, full-duplex, 4-wire serial bus:
- **MOSI:** Master Out Slave In
- **MISO:** Master In Slave Out
- **SCLK:** Serial Clock
- **SS/CS:** Slave Select / Chip Select (Active Low)

### Clock Polarity (CPOL) and Clock Phase (CPHA)
These settings define when data is sampled and when it is shifted:
- **CPOL = 0:** Clock idle state is Low.
- **CPOL = 1:** Clock idle state is High.
- **CPHA = 0:** Data sampled on the 1st clock edge (leading edge).
- **CPHA = 1:** Data sampled on the 2nd clock edge (trailing edge).

### SPI Mode Table
| SPI Mode   | CPOL | CPHA | Sample Edge  | Shift Edge   |
|------------|------|------|--------------|--------------|
| **Mode 0** | 0    | 0    | Rising Edge  | Falling Edge |
| **Mode 1** | 0    | 1    | Falling Edge | Rising Edge  |
| **Mode 2** | 1    | 0    | Falling Edge | Rising Edge  |
| **Mode 3** | 1    | 1    | Rising Edge  | Falling Edge |

### ASCII Timing Diagram: SPI Mode 0 (CPOL=0, CPHA=0)
Data is driven/shifted on the falling edge of SCLK and sampled on the rising edge.
```
CS:   ───┐                                 ┌───
         └─────────────────────────────────┘
SCLK: ───────┐   ┌───┐   ┌───┐   ┌───┐   ┌───── (CPOL=0)
             └───┘   └───┘   └───┘   └───┘
MOSI: ───[ Bit 7 ]───[ Bit 6 ]───[ Bit 5 ]───── (Shifted on SCLK fall)
MISO: ───[ Bit 7 ]───[ Bit 6 ]───[ Bit 5 ]─────
         ^ Sampleed on SCLK rise
```

### Multi-Master SPI
If multiple masters share the SPI bus, they must use hardware chip select pins or software arbitration to ensure only one master drives the SCLK, MOSI, and SS lines at any given time. Otherwise, short circuits and bus contention occur.

### SPI Daisy Chain
Instead of separate CS lines for each slave, slaves are connected in series. The MISO of Slave 1 goes to MOSI of Slave 2, and so on. A single CS line controls all slaves. Data overflows from one slave to the next like a giant shift register.

```
MOSI ──►[Slave 1]──(MISO to MOSI)──►[Slave 2]──► MISO (to Master)
CS   ──┬─────────────────────────────┘
SCLK ──┴─────────────────────────────┘
```

### Quad-SPI (Q-SPI)
Q-SPI uses 4 data lines (IO0, IO1, IO2, IO3) instead of single MOSI/MISO lines. This allows transferring 4 bits per clock cycle, quadrupling throughput. Used primarily for interfacing with high-speed external Flash memories.

---

## Q5: UART Protocol Frame Format & Flow Control

UART (Universal Asynchronous Receiver-Transmitter) uses two lines: TX and RX.

### Frame Format
```
Idle (High) ──┐      ┌───┬───┬───┬───┬───┬───┬───┬───┬───────┬───────┐
              │      │ D0│ D1│ D2│ D3│ D4│ D5│ D6│ D7│Parity │ Stop  │─── Idle (High)
              └──────┴───┴───┴───┴───┴───┴───┴───┴───┴───────┴───────┘
              ^ START  (Data Bits, LSB first)                 ^ STOP
                (Low)                                           (High)
```

### RTS/CTS Hardware Flow Control
Prevents buffer overrun:
- **RTS (Request to Send):** Receiver pulls this Low when its buffer is empty, indicating it is ready to receive data.
- **CTS (Clear to Send):** Transmitter checks this line. If the receiver pulls it Low, the transmitter begins sending data.

---

## Q6: CAN Bus Protocol (Arbitration, Frame, Errors)

CAN (Controller Area Network) is a differential, multi-master, asynchronous serial bus.

### CAN Bit-by-Bit Arbitration
CAN uses dominant bits (`0`, differential voltage) and recessive bits (`1`, zero differential). If two nodes transmit at the same time, the node transmitting a dominant bit (`0`) overwrites the recessive bit (`1`) of the other. **Consequently, the message with the lower numerical ID has higher priority.**

```
Node A (ID 0x0A = 0000 1010):  0 0 0 0 1 0 1 0
Node B (ID 0x0C = 0000 1100):  0 0 0 0 1 1 [Loses Bus! Stops transmitting]
Bus State:                     0 0 0 0 1 0 1 0
```

### CAN Frame Format
- **SOF:** Start of Frame (1 dominant bit).
- **Arbitration Field:** 11-bit identifier + RTR (Remote Transmission Request).
- **Control Field:** IDE (Identifier Extension) + r0 (Reserved) + DLC (Data Length Code).
- **Data Field:** 0 to 8 bytes.
- **CRC Field:** 15-bit Cyclic Redundancy Check + CRC Delimiter.
- **ACK Field:** ACK Slot (Receiver drives dominant) + ACK Delimiter.
- **EOF:** End of Frame (7 recessive bits).

### 5 CAN Error Types
1. **Bit Error:** A node reads back a different bit value than what it transmitted.
2. **Stuff Error:** Violating the bit-stuffing rule (more than 5 consecutive identical bits).
3. **CRC Error:** Calculated CRC does not match the received frame CRC.
4. **Form Error:** Invalid structure inside standard fields (like delimiters or EOF).
5. **ACK Error:** Transmitter does not detect a dominant bit in the ACK slot.

---

## Q7: GPIO Initialization (STM32 Register-Level)

### Configurable Parameters
- **Mode:** Input, Output, Alternate Function, Analog.
- **Output Type:** Push-Pull (drives high/low) or Open-Drain (needs pull-up).
- **Pull-Up/Pull-Down:** Activates internal weak pull resistors.
- **Speed:** Output slew rate (Low, Medium, High).

### Code Example (STM32 Cortex-M4 register-level C)
```c
#include <stdint.h>

// Register Definitions for GPIOC on STM32F4
#define RCC_AHB1ENR     (*(volatile uint32_t*)(0x40023800 + 0x30))
#define GPIOC_MODER     (*(volatile uint32_t*)(0x40020800 + 0x00))
#define GPIOC_OTYPER    (*(volatile uint32_t*)(0x40020800 + 0x04))
#define GPIOC_PUPDR     (*(volatile uint32_t*)(0x40020800 + 0x0C))

void GPIO_Init_PC13_Output(void) {
    // 1. Enable Clock for GPIOC (Bit 2 of RCC AHB1ENR)
    RCC_AHB1ENR |= (1 << 2);
    
    // 2. Set Pin 13 to General Purpose Output (01 in MODER register)
    GPIOC_MODER &= ~(3 << (13 * 2)); // Clear mode bits
    GPIOC_MODER |=  (1 << (13 * 2)); // Set mode to output
    
    // 3. Set PC13 Output Type to Push-Pull (0 in OTYPER register)
    GPIOC_OTYPER &= ~(1 << 13);
    
    // 4. Set No Pull-Up / No Pull-Down (00 in PUPDR register)
    GPIOC_PUPDR &= ~(3 << (13 * 2));
}
```

---

## Q8: Software Debouncing Logic

Mechanical switches bounce when pressed, causing false triggers. Software debouncing filters this by sampling the pin over a period.

```
Switch State: High ───┐   ┌─┐   ┌─┐   ┌───────── Low (Pressed)
                      └───┘ └───┘ └───┘
                      <── Bouncing ──> <── Stable ──>
```

---

## Q9: ADC Voltage Divider Calculation

### Problem
Read 20V input using a 10-bit ADC with a reference voltage (\(V_{ref}\)) of 3.5V.

### Solution
Directly connecting 20V will destroy the MCU. We must use a voltage divider to scale 20V down to a maximum of 3.5V.



## Q10: DMA (Direct Memory Access)

DMA allows hardware peripherals to send/receive data directly to/from the main RAM without involving the CPU. This frees up the CPU to perform other operations.

```
       +-----+                  +--------+
       | CPU |◄─── DMA Bus ────►| Memory |
       +-----+    Arbitration   +--------+
          │                         ▲
     DMA Control                    │  DMA Transfer
          ▼                         │
     +────────+                     │
     | DMA    |─────────────────────┘
     | Control|◄── Request
     +────────+
          ▲
          │ Handshake
          ▼
     +────────────+
     | Peripheral |
     +────────────+
```

### Solving High-Speed Byte-by-Byte Interrupt problem
If a peripheral triggers an interrupt for every incoming byte, the CPU suffers from high interrupt overhead (context switching latency). 
**Solution:** Configure the DMA controller in Circular/Double-buffer mode to transfer incoming bytes automatically into a RAM buffer. Set the DMA to trigger an interrupt only when the buffer is half-full or completely full (Transfer Complete interrupt).

---

## Q11: Microcontroller Startup Sequence

Before `main()` is executed, the following actions occur:

1. **Hardware Reset:** Power applied or reset pin toggles. CPU reads the vector table starting at flash memory address `0x00000000` (or `0x08000000` on STM32).
2. **MSP Load:** Loads the Initial Stack Pointer value from the first entry of the vector table into the SP register.
3. **Program Counter Load:** Loads the address of the `Reset_Handler` (second entry of the vector table) into the PC register.
4. **Execution of `Reset_Handler` (Startup Assembly code):**
   - Copies initialized global/static variable values from Flash to SRAM (`.data` segment).
   - Zeroes out the `.bss` segment in SRAM.
   - Configures system clocks (PLL setup via `SystemInit()`).
5. **Jump to `main`:** Jump instruction calls the `main()` function.
