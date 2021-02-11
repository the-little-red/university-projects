DESCRIPTION
===========

This package is responsible for activating and deactivating the automatic 
updates system of Linux Educacional 6. The LE 6 has a system that updates 
daily all its packages with available updates. This feature is used so that 
manual updates are not necessary. The auto updates can be deactivated in 
the LE 6 Control Panel.

LE 6 auto-upgrade will set up the system to check for updates daily. This 
package will configure unattended-upgrades (with the le-autoupgrade script) 
to verify everyday for new upgrades in the LE6 repository, as well as Ubuntu 
16.04 repository. Some options are also changed in dpkg and APT with the 
conffile 99le6-auto-upgrade.conf to ensure that the package works properly.

Requirements
===========

unattended-upgrades le-help
