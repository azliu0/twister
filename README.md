# Twister

A binary exploitation challenge for HackMIT Puzzle 2024

To run exploit script locally:
- `make`
- move twister binary into root dir
- `sudo chown root:root twister`
- `sudo chmod 4655 twister`

## Deployment

The docker container is deployed on another instance running the script `scripts/deploy.sh` every 12 hours via crontab.
