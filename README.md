# gpio-mockup Kernel Module

This is a simple GPIO mockup kernel module that registers a fake GPIO chip with 4 GPIOs. The purpose of this module is to demonstrate how to create and manage GPIOs in the Linux kernel and how to interact with them using `gpiod` tools like `gpiodetect` and `gpioinfo`.

## Features

- Registers a mock GPIO chip with 4 GPIOs.
- Provides basic functionality for getting and setting GPIO values, as well as changing GPIO directions (input/output).
- Outputs logs when GPIO operations are performed.

## Requirements

- Linux Kernel (with GPIO subsystem support)
- `gpiod` tools (installed via `libgpiod`)

## Build Requirements

- Compiler
- Linux-headers

Various Linux distrobutions have their own idea about Linux-headers.
On Debian there is the package `linux-headers-amd64`.
Verify your distro with commands like `apt search linux-header`
and `apt show linux-header-amd64`.

### Installing `libgpiod`

On Ubuntu, install `libgpiod` tools by running:

```shell
sudo apt install gpiod
```

This will install tools like gpiodetect, gpioinfo, gpioset, and gpioget.

## Building and Installing the Module

1. Clone the repository or download the source files.
2. Compile the module using the provided `Makefile`:
```shell
make
```
3. Insert the module using `insmod`:
``` shell
sudo insmod gpio-mockup.ko
```
4. Verify the module is loaded by checking `dmesg`:
```shell
sudo dmesg | grep "Mockup GPIO"
```
5. Confirm that the mock GPIO chip is registered using `gpiodetect`:
```shell
sudo gpiodetect
```
You should see a new GPIO chip called mockup_gpio with 4 lines.

## Using gpiod Tools
### List the GPIO Chips (gpiodetect)
To check if the mock GPIO chip is registered, use:

```shell
sudo gpiodetect
```
Example output:

```shell
$ sudo gpiodetect
gpiochip0 [mockup_gpio] (4 lines)
This shows that the GPIO chip mockup_gpio is registered with 4 GPIO lines.
```

### Get Information on GPIO Lines (gpioinfo)
To get detailed information about the GPIO lines, use:

```shell
sudo gpioinfo mockup_gpio
```

Example output:

```shell
$ sudo gpioinfo mockup_gpio
gpiochip0 - 4 lines:
    line   0: unnamed unused input active-high
    line   1: unnamed unused input active-high
    line   2: unnamed unused input active-high
    line   3: unnamed unused input active-high
```

### Set GPIO Output Value (gpioset)
You can set the output value of the GPIO lines using the gpioset tool. For example, to set GPIO 0 to high (1), run:

```shell
sudo gpioset mockup_gpio 0=1
```

You can verify the value was set using gpioinfo or by reading the value with gpioget (after setting the direction to output).

### Get GPIO Input Value (gpioget)
To get the value of a specific GPIO pin, use the gpioget tool. For example, to get the value of GPIO 0:

```shell
sudo gpioget mockup_gpio 0
```
This will return the current value of GPIO 0, which should be 1 if you previously set it to high.

### Change GPIO Direction (gpioset)
To change the direction of a GPIO pin (e.g., set GPIO 1 as an output and set its value to 1), use:

```shell
sudo gpioset --mode=signal mockup_gpio 1=1
```
You can also check the status using gpioinfo to confirm the direction and value.

## Unloading the Module
To unload the module, use:

```shell
sudo rmmod gpio-mockup
```

## Reference
- [libgpiod/tests/mockup/gpio-mockup.c](https://github.com/brgl/libgpiod/blob/v1.6.x/tests/mockup/gpio-mockup.c)
- [linux/drivers/gpio/gpio-mockup.c](https://github.com/torvalds/linux/blob/master/drivers/gpio/gpio-mockup.c)

## TODO
- [ ] Import debugfs to set the behavior of the gpiochip.

## License
This module is licensed under the GPL. See the LICENSE file for details.

Author
Hank Wang - wanghanchi2000@gmail.com


