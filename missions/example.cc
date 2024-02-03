#include <FreeRTOS.h>
#include <task.h>

/**
 * @brief Example main task.
 *
 * This is a FreeRTOS task defined in CubeMX codegen as the entry of application
 * code. Its implementation controls the behavior of the robot and affords the
 * robot the ability to accomplish a mission.
 *
 * When this task is scheduled, safely assume that hardware and OS has been
 * initialized. Use this task to spawn other tasks and to perform business
 * logic of the robotic software.
 *
 * Define a new mission (i.e. robot application) by providing a different
 * implementation of this task with the exact same signature in a new source
 * file. In every translation unit, this function symbol needs to be marked
 * "extern C" to make it linkable with auto generated C code. And it should also
 * be marked "weak" to avoid duplicated definitions causing a linker error.
 *
 * When building the executable with this main task, remove "weak" attribute to
 * instruct the linker to link against this definition.
 *
 * @attention As a FreeRTOS task, this function must not return.
 */
extern "C" __attribute__((weak)) void main_task(const void*) {
  while (true) {
    vTaskDelay(10U);
  }
}
