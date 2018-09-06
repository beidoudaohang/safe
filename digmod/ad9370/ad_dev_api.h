#ifndef AD_DEV_API_H_
#define AD_DEV_API_H_

typedef enum {
	ERR_NODEV = 1,
	ERR_OPENFALSE,
	ERR_NOOPEN,
	ERR_CONFIGFALSE
} AD_DEV_ERR;

struct ad_spi_cfg {
	uint8_t mode;
	uint8_t bits;
	uint32_t speed;
	uint16_t delay;
};

int ad_dev_write(int fd, int reg, char dat, struct ad_spi_cfg *cfg);
char ad_dev_read(int fd, int reg, struct ad_spi_cfg *cfg);
#endif //AD_DEV_API_H_