#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device *dev = DEVICE_DT_GET_ANY(my_led_sensor);

    if (!device_is_ready(dev)) {
        LOG_ERR("Device not ready");
        return 0;
    }

    LOG_INF("Device is ready");

    bool led_state = false;
    struct sensor_value val;

    while(1)
    {
        if(led_state)
        {
            sensor_sample_fetch(dev);
        }
        else
        {
            sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
        }

        led_state = !led_state;
        k_msleep(1000);
    }

    return 0;
}
