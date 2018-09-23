/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.h - Configuration routines
 */

#ifndef CONFIG_H
#define CONFIG_H

#define IO_MODE_SERIAL        0

#define SER_BAUD_19200        0x00
#define SER_BAUD_38400        0x01
#define SER_BAUD_1200         0x04
#define SER_BAUD_2400         0x05
#define SER_BAUD_9600         0x07

typedef struct configInfo
{
  unsigned char io_mode;
  unsigned char baud;
  unsigned short port;
} ConfigInfo;

/**
 * config_init()
 * Initialize configuration and load either config or defaults.
 */
void config_init(void);

/**
 * config_init_hook()
 * Special system specific initialization functions.
 */
void config_init_hook(void);

/**
 * config_load()
 * Load the configuration file, or if not found, set some defaults and save.
 */
void config_load(void);

/**
 * config_save()
 * Save the configuration file.
 */
void config_save(void);

/**
 * config_set_defaults()
 * Set default values for config file
 */
void config_set_defaults(void);

#endif /* CONFIG_H */
