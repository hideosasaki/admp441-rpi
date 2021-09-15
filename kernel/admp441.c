#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#define ADMP441_RATE_MIN 16000
#define ADMP441_RATE_MAX 48000

#define ADMP441_FORMATS (SNDRV_PCM_FMTBIT_S32_LE | SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S16_LE)

static struct snd_soc_dai_driver admp441_dai = {
	.name = "admp441-hifi",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rate_min = ADMP441_RATE_MIN,
		.rate_max = ADMP441_RATE_MAX,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.formats = ADMP441_FORMATS,
	},
};

static struct snd_soc_dai admp441_soc_driver = { };

static struct snd_soc_component_driver admp441_soc_component = {
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
	.non_legacy_dai_naming	= 1,
};

static int admp441_probe(struct platform_device *pdev)
{
	admp441_soc_driver = *devm_snd_soc_register_dai(&pdev->dev, &admp441_soc_component, &admp441_dai, 1);
	return 0;
}

static int admp441_remove(struct platform_device *pdev)
{
	snd_soc_unregister_dai(&admp441_soc_driver);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id admp441_ids[] = {
	{ .compatible = "invensense,admp441", },
	{ }
};
MODULE_DEVICE_TABLE(of, admp441_ids);
#endif

static struct platform_driver admp441_driver = {
	.driver = {
		.name = "admp441",
		.of_match_table = of_match_ptr(admp441_ids),
	},
	.probe = admp441_probe,
	.remove = admp441_remove,
};

module_platform_driver(admp441_driver);

MODULE_DESCRIPTION("ADMP441 driver");
MODULE_AUTHOR("George Talusan <george.talusan@gmail.com>, hideosasaki");
MODULE_LICENSE("GPL v2");
