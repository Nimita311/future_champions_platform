# Future Champions Platform

Educational robotic platform made for future champions.

Built on [NUCLEO-H743ZI2](https://estore.st.com/en/nucleo-h743zi2-cpn.html) powered by [Arm Cortex-M7](https://developer.arm.com/Processors/Cortex-M7).

## Getting Started

1. Clone this repo including submodules.
    ```
    git clone --recurse-submodules https://github.com/Nimita311/future_champions_platform.git
    ```

1. Download [STM32CubeIDE](https://www.st.com/content/st_com/en/stm32cubeide.html) and import this project.
    ```
    File -> Import... -> Existing Projects into Workspace
    ```

1. Open `fcp_mcu.ioc` in STM32CubeIDE and generate code.
    ```
    Project -> Generate Code
    ```

1. Browse and select a demo under [`missions/`](./missions/) you want to build. Remove `__attribute__((weak))` in front of `main_task()` to link with it.

1. Build, program, and debug on chip with STM32CubeIDE. Edit code with [Visual Studio Code](https://code.visualstudio.com).
