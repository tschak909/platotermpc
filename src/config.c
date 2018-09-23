/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.c - Configuration routines
 */

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "screen.h"

#define CONFIG_FILE "config.cfg"

ConfigInfo config;
char configFile[13];

/**
 * config_init()
 * Initialize configuration and load either config or defaults.
 */
void config_init(void)
{
  strcpy(configFile,CONFIG_FILE);
  memset(&config,0,sizeof(config));
  config_load();
}

/**
 * config_load()
 * Load the configuration file, or if not found, set some defaults and save.
 */
void config_load(void)
{
  FILE *fp;
  fp = fopen(configFile,"r");
  
  if (!fp)
    {
      config_set_defaults();
    }
  else
    {
      fread(&config,1,sizeof(config),fp);
    }
  fclose(fp);
}

/**
 * config_save()
 * Save the configuration file.
 */
void config_save(void)
{
  FILE *fp;
  fp = fopen(configFile,"w");

  if (!fp)
    return;

  fwrite(&config,1,sizeof(config),fp);
  fclose(fp);
}

/**
 * config_set_defaults()
 * Set default values for config file
 */
void config_set_defaults(void)
{
  config.io_mode = IO_MODE_SERIAL;
  config.baud = SER_BAUD_1200;
  config.port=0; // COM1
  config_save();
}
