
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT my_led_sensor

/* Chamada correta para registo do módulo de log (sem #define) */
LOG_MODULE_REGISTER(my_driver, CONFIG_SENSOR_LOG_LEVEL);

#define LED_NODE DT_ALIAS(app_led)

#if !DT_NODE_HAS_STATUS_OKAY(LED_NODE)
#error "Unsupported board: app_led devicetree alias is not defined"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

/* TASK 1: Fetch LIGA o LED */
static int my_led_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    gpio_pin_set_dt(&led, 1);
    LOG_INF("Turning on the led");
    return 0;
}

/* TASK 1: Get DESLIGA o LED */
static int my_led_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    gpio_pin_set_dt(&led, 0);
    LOG_INF("Turning off the led");
    if (val != NULL) {
        val->val1 = 0;
        val->val2 = 0;
    }
    return 0;
}

static DEVICE_API(sensor, api_my_driver) = {
    .sample_fetch = my_led_sample_fetch,
    .channel_get = my_led_channel_get,
};

static int led_sensor_init(const struct device *dev)
{
    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED device is not ready");
        return -ENODEV;
    }
    return gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
}

/* Instanciação direta para a instância 0 (índice 0 como 1º argumento) */
DEVICE_DT_INST_DEFINE(0,
                      led_sensor_init,
                      NULL,
                      NULL,
                      NULL,
                      POST_KERNEL,
                      CONFIG_SENSOR_INIT_PRIORITY,
                      &api_my_driver);