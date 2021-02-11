% LE-AUTOUPGRADE(8) Version 1.0 | System Manager Manual

NAME
====

**le-autoupgrade**, **unattended-upgrade(8)** — automatic installation of security (and other) upgrades in Educacional Linux.

SYNOPSIS
========

| **unattended-upgrade** \[options]

DESCRIPTION
===========

This program can download and install security upgrades automatically and
unattended, taking care to only install packages from the configured
APT source, and checking for dpkg  prompts about configuration file changes.
All output is logged to /var/log/unattended-upgrades.log.

This  script is the backend for the APT::Periodic::Unattended-Upgrade option and designed to
be run from cron (e.g. via /etc/cron.daily/apt).

This package version is a modified version of unattended-upgrades, the changes set
the package to enable the updates automatically and to this updates happen on random times of the day.

Options
-------

-h, --help

:   help output

-d, --debug

:   extra debug output into /var/log/unattended-upgrades.log

--dry-run

:   just simulate installing updates, do not actually do it

AUTHORS
=====

unattended-upgrades is written by Michael Vogt <mvo@ubuntu.com>

This modified version patched as le-autoupgrade was done by Arianne Bortolan <apb14@inf.ufpr.br>

SEE ALSO
========

**unattended-upgrade(8)**
