#include <zephyr/device.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

/**
 * fetch — calls sensor_sample_fetch()
 * read — calls sensor_channel_get() and prints the result
 * info — prints the device name and ready state
 */


static int cmd_shell_fetch(const struct shell *shell, size_t argc, char **argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);
        
    if(!dev) {
        shell_error(shell, "Device not found: %s", argv[1]);
        return -EFAULT;
    }
    
    if(sensor_sample_fetch(dev) != 0) {
        shell_error(shell, "Failed to fetch sample from device: %s", argv[1]);
        return -EFAULT;
    }

    shell_info(shell, "Fetch done: LED on");

    return 0;
}

static int cmd_shell_read(const struct shell *shell, size_t argc, char **argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);

    if(!dev) {
        shell_error(shell, "Device not found: %s", argv[1]);
        return -EFAULT;
    }

    struct sensor_value val;

    if(sensor_channel_get(dev, SENSOR_CHAN_ALL, &val) != 0) {
        shell_error(shell, "Failed to read from device: %s", argv[1]);
        return -EFAULT;
    }

    shell_info(shell, "Read done: LED off, value = %d.%06d", val.val1, val.val2);

    return 0;
}

static int cmd_shell_info(const struct shell *shell, size_t argc, char **argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);

    if(!dev) {
        shell_error(shell, "Device not found: %s", argv[1]);
        return -EFAULT;
    }
    
    shell_info(shell, "Device name: %s", dev->name);
    shell_info(shell, "Device state: %s", device_is_ready(dev) ? "ready" : "not ready");

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_my_driver,
    SHELL_CMD_ARG(fetch, NULL, "Turn LED on",  cmd_shell_fetch, 2, 0),
    SHELL_CMD_ARG(read,  NULL, "Turn LED off", cmd_shell_read,  2, 0),
    SHELL_CMD_ARG(info,  NULL, "Name and state", cmd_shell_info, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensorroot, &sub_my_driver, "My driver commands", NULL);