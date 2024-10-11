/**
 *  GPIO Testing Device Driver
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/gpio/driver.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#define MOCKUP_GPIO_NUM 4

static struct gpio_chip mockup_gpio_chip;
static int gpio_values[MOCKUP_GPIO_NUM] = {
    0}; // Array to store the state of each GPIO pin

/**
 * mockup_gpio_get_direction - Get the direction of a GPIO pin.
 * @chip: Pointer to the GPIO chip structure.
 * @offset: GPIO pin number.
 *
 * Returns 0, assuming all pins are in output mode by default.
 * Checks if the pin offset is valid before proceeding.
 */
static int mockup_gpio_get_direction(struct gpio_chip *chip,
                                     unsigned int offset) {
  if (offset >= MOCKUP_GPIO_NUM) {
    pr_err("Mockup GPIO: Invalid GPIO pin offset %d\n", offset);
    return -EINVAL;
  }
  return 0;
}

/**
 * mockup_gpio_direction_input - Set the direction of a GPIO pin to input.
 * @chip: Pointer to the GPIO chip structure.
 * @offset: GPIO pin number.
 *
 * Logs the operation and checks for a valid pin offset.
 */
static int mockup_gpio_direction_input(struct gpio_chip *chip,
                                       unsigned int offset) {
  if (offset >= MOCKUP_GPIO_NUM) {
    pr_err("Mockup GPIO: Invalid GPIO pin offset %d\n", offset);
    return -EINVAL;
  }
  pr_info("Mockup GPIO: Set direction to input for pin %d\n", offset);
  return 0;
}

/**
 * mockup_gpio_direction_output - Set the direction of a GPIO pin to output.
 * @chip: Pointer to the GPIO chip structure.
 * @offset: GPIO pin number.
 * @value: Value to set the pin to after direction change (0 or 1).
 *
 * Logs the operation, checks for a valid pin offset, and sets the value of the
 * pin.
 */
static int mockup_gpio_direction_output(struct gpio_chip *chip,
                                        unsigned int offset, int value) {
  if (offset >= MOCKUP_GPIO_NUM) {
    pr_err("Mockup GPIO: Invalid GPIO pin offset %d\n", offset);
    return -EINVAL;
  }
  pr_info("Mockup GPIO: Set direction to output for pin %d, value %d\n", offset,
          value);
  gpio_values[offset] = value;
  return 0;
}

/**
 * mockup_gpio_get - Get the value of a GPIO pin.
 * @chip: Pointer to the GPIO chip structure.
 * @offset: GPIO pin number.
 *
 * Logs the operation and checks if the pin offset is valid.
 * Returns the value stored in gpio_values[].
 */
static int mockup_gpio_get(struct gpio_chip *chip, unsigned int offset) {
  if (offset >= MOCKUP_GPIO_NUM) {
    pr_err("Mockup GPIO: Invalid GPIO pin offset %d\n", offset);
    return -EINVAL;
  }
  pr_info("Mockup GPIO: Get value of pin %d: %d\n", offset,
          gpio_values[offset]);
  return gpio_values[offset];
}

/**
 * mockup_gpio_set - Set the value of a GPIO pin.
 * @chip: Pointer to the GPIO chip structure.
 * @offset: GPIO pin number.
 * @value: Value to set the pin to (0 or 1).
 *
 * Logs the operation, checks for a valid pin offset, and sets the value in
 * gpio_values[].
 */
static void mockup_gpio_set(struct gpio_chip *chip, unsigned int offset,
                            int value) {
  if (offset >= MOCKUP_GPIO_NUM) {
    pr_err("Mockup GPIO: Invalid GPIO pin offset %d\n", offset);
    return;
  }
  pr_info("Mockup GPIO: Set value of pin %d to %d\n", offset, value);
  gpio_values[offset] = value;
}

/**
 * mockup_gpio_probe - Probe function for the GPIO mockup driver.
 * @pdev: Platform device pointer.
 *
 * Initializes and registers the GPIO chip using devm_gpiochip_add_data.
 * Returns 0 on success or a negative error code on failure.
 */
static int mockup_gpio_probe(struct platform_device *pdev) {
  int ret;

  /* Initialize GPIO chip structure */
  mockup_gpio_chip.label = "mockup_gpio";
  mockup_gpio_chip.parent = &pdev->dev;
  mockup_gpio_chip.owner = THIS_MODULE;
  mockup_gpio_chip.base = -1; /* Dynamic base allocation */
  mockup_gpio_chip.ngpio = MOCKUP_GPIO_NUM;
  mockup_gpio_chip.get_direction = mockup_gpio_get_direction;
  mockup_gpio_chip.direction_input = mockup_gpio_direction_input;
  mockup_gpio_chip.direction_output = mockup_gpio_direction_output;
  mockup_gpio_chip.get = mockup_gpio_get;
  mockup_gpio_chip.set = mockup_gpio_set;

  /* Register GPIO chip with device-managed resource handling */
  ret = devm_gpiochip_add_data(&pdev->dev, &mockup_gpio_chip, NULL);
  if (ret) {
    dev_err(&pdev->dev, "Failed to register GPIO chip: %d\n", ret);
    return ret;
  }

  pr_info("Mockup GPIO chip registered\n");
  return 0;
}

/**
 * mockup_gpio_remove - Remove function for the GPIO mockup driver.
 * @pdev: Platform device pointer.
 *
 * Unregisters the GPIO chip.
 */
static int mockup_gpio_remove(struct platform_device *pdev) {
  pr_info("Mockup GPIO chip unregistered\n");
  return 0;
}

/* Platform driver structure */
static struct platform_driver mockup_gpio_driver = {
    .probe = mockup_gpio_probe,
    .remove = mockup_gpio_remove,
    .driver =
        {
            .name = "gpio-mockup",
        },
};

static struct platform_device *mockup_gpio_device;

/**
 * mockup_gpio_init - Module init function.
 *
 * Allocates and registers the platform device and driver.
 * Returns 0 on success or a negative error code on failure.
 */
static int __init mockup_gpio_init(void) {
  int ret;

  /* Allocate and register platform device */
  mockup_gpio_device = platform_device_alloc("gpio-mockup", -1);
  if (!mockup_gpio_device)
    return -ENOMEM;

  ret = platform_device_add(mockup_gpio_device);
  if (ret) {
    platform_device_put(mockup_gpio_device);
    return ret;
  }

  /* Register platform driver */
  ret = platform_driver_register(&mockup_gpio_driver);
  if (ret) {
    platform_device_unregister(mockup_gpio_device);
    return ret;
  }

  pr_info("Mockup GPIO module loaded\n");
  return 0;
}

/**
 * mockup_gpio_exit - Module exit function.
 *
 * Unregisters the platform driver and device.
 */
static void __exit mockup_gpio_exit(void) {
  platform_driver_unregister(&mockup_gpio_driver);
  platform_device_unregister(mockup_gpio_device);
  pr_info("Mockup GPIO module unloaded\n");
}

module_init(mockup_gpio_init);
module_exit(mockup_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hank Wang - wanghanchi2000@gmail.com");
MODULE_DESCRIPTION("gpio-mockup example");
