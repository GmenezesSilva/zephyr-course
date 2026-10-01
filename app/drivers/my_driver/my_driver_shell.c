#include <zephyr/device.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

#define MODE_MIN 0
#define MODE_MAX 2

int my_led_sensor_set_mode(const struct device *dev, int mode);

static int cmd_shell_fetch(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = shell_device_get_binding(argv[1]);

	if (!dev) {
		shell_error(sh, "Device not found: %s", argv[1]);
		return -EFAULT;
	}

	if (sensor_sample_fetch(dev) != 0) {
		shell_error(sh, "Failed to fetch sample from device: %s", argv[1]);
		return -EFAULT;
	}

	shell_info(sh, "Fetch done: LED on");
	return 0;
}

static int cmd_shell_read(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = shell_device_get_binding(argv[1]);

	if (!dev) {
		shell_error(sh, "Device not found: %s", argv[1]);
		return -EFAULT;
	}

	struct sensor_value val;

	if (sensor_channel_get(dev, SENSOR_CHAN_ALL, &val) != 0) {
		shell_error(sh, "Failed to read from device: %s", argv[1]);
		return -EFAULT;
	}

	shell_info(sh, "Read done: LED off, value = %d.%06d", val.val1, val.val2);
	return 0;
}

static int cmd_shell_info(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = shell_device_get_binding(argv[1]);

	if (!dev) {
		shell_error(sh, "Device not found: %s", argv[1]);
		return -EFAULT;
	}

	shell_info(sh, "Device name: %s", dev->name);
	shell_info(sh, "Device state: %s", device_is_ready(dev) ? "ready" : "not ready");
	return 0;
}

static int cmd_shell_set(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = shell_device_get_binding(argv[1]);

	if (!dev) {
		shell_error(sh, "Device not found: %s", argv[1]);
		return -EFAULT;
	}

	if (argc < 3) {
		shell_error(sh, "Missing value. Usage: set <device> <value>");
		return -EINVAL;
	}

	int mode = atoi(argv[2]);

	if (mode < 0 || mode > 1) {
		shell_error(sh, "Value out of range (0..1): %d", mode);
		return -ERANGE;
	}

	if (my_led_sensor_set_mode(dev, mode) != 0) {
		shell_error(sh, "Failed to set mode");
		return -EFAULT;
	}

	shell_info(sh, "Mode set to %d", mode);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
	SHELL_CMD_ARG(fetch, NULL, "fetch <device>",       cmd_shell_fetch, 2, 0),
	SHELL_CMD_ARG(read,  NULL, "read <device>",        cmd_shell_read,  2, 0),
	SHELL_CMD_ARG(info,  NULL, "info <device>",        cmd_shell_info,  2, 0),
    SHELL_CMD_ARG(set,   NULL, "set <device> <value>", cmd_shell_set,   3, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Sensor root commands", NULL);