# Valgrind &ndash; RISCV64/Linux

## Overview

This repository contains [Valgrind][Valgrind] with support for the
[RISCV64][RISC-V]/[Linux][Linux] platform.

The aim of the project is to enable Valgrind for the RV64GC instruction set on
the Linux operating system. Once this support is implemented and has sufficient
quality, it is intended for this port to become a part of the upstream Valgrind
project and to continue further development there.

For installation, please follow the generic steps how to build Valgrind in [the
main README file](README).

In case you are interested in helping with the port then the best option is to
analyze remaining failing tests in the Valgrind test suite, or just try the port
with your application and report any discovered bugs.

The project was presented at [FOSDEM 2022][FOSDEM talk].

## Current state

Current focus is on functionality and correctness.

The following tables summarize the basic state as of 2023-08-29.

The following ISA base and extensions are currently supported:

| Name         | Description                       | #Instrs | Notes    |
| ------------ | --------------------------------- | ------- | -------- |
| RV64I        | Base instruction set              |   52/52 |          |
| RV64M        | Integer multiplication & division |   12/13 | (1)      |
| RV64A        | Atomic                            |   22/22 | (2)      |
| RV64F        | Single-precision floating-point   |   30/30 | (3)      |
| RV64D        | Double-precision floating-point   |   32/32 |          |
| RV64Zicsr    | Control & status register         |     2/6 | (4), (5) |
| RV64Zifencei | Instruction-fetch fence           |     0/1 | (6)      |
| RV64C        | Compressed                        |   37/37 |          |
| RV64Zfh      | Half-precision floating-point     |   ALL   |          |
| RV64Xthead   | T-THEAD vendor extension for RV64G|   ALL   | (7)      |
| RV64V0p7     | Vector 0.7.1                      |   ALL   | (8)      |
| RV64V        | Vector 1.0                        |   ALL   | (9)      |

Notes:

(1) MULHSU is not recognized.

(2) LR and SC use the VEX "fallback" method which suffers from the ABA problem.

(3) Operations do not check if the input operands are correctly NaN-boxed.

(4) CSRRC, CSRRWI, CSRRSI and CSRRCI are not recognized.

(5) Only registers fflags, frm and fcsr are accepted.

(6) FENCE.I is not recognized.

(7) See https://github.com/T-head-Semi/thead-extension-spec

(8) See https://github.com/riscv/riscv-v-spec/releases/tag/0.7.1

(9) See https://github.com/riscv/riscv-v-spec/releases/tag/v1.0

Test results:

| Test set   | #Failed/#Total |
| ---------- | -------------: |
| Memcheck   |         0/219 |
| Nulgrind   |          0/140 |
| Cachegrind |            0/7 |
| Callgrind  |           0/15 |
| DHAT       |            0/8 |
| DRD        |          0/130 |
| Helgrind   |           0/55 |
| Massif     |           0/37 |
| GDBserver  |           0/25 |

## License

This project is released under the terms of [the GPLv2 License](COPYING).

[Valgrind]: https://valgrind.org/
[RISC-V]: https://riscv.org/
[Linux]: https://github.com/torvalds/linux
[FOSDEM talk]: https://archive.fosdem.org/2022/schedule/event/valgrind_riscv/
