# project settings
TOP := $(shell pwd)

#
# Vagrant
#

.PHONY: converge destroy

# create and provision the vagrant box
converge:
	vagrant up --provision

# stop and destroy the vagrant box
destroy:
	vagrant destroy -f

#
# Testing
#

# XXX: maybe reduce repetition with: https://stackoverflow.com/a/26522658/937006

.PHONY: test-part1 test-part2 test

# configure the vagrant box for part1 and run tests
test-part1: converge
	vagrant ssh-config > $(TOP)/.vagrant/ssh-config
	ansible-playbook --ssh-common-args '-F .vagrant/ssh-config' -i 'default,' part1.yaml
	py.test --connection=ssh --ssh-config=$(TOP)/.vagrant/ssh-config --hosts=default part1/test_daemon.py

# configure the vagrant box for part2 and run tests
test-part2: converge
	vagrant ssh-config > $(TOP)/.vagrant/ssh-config
	ansible-playbook --ssh-common-args '-F .vagrant/ssh-config' -i 'default,' part2.yaml
	py.test --connection=ssh --ssh-config=$(TOP)/.vagrant/ssh-config --hosts=default part2/test_daemon.py

# run all available tests
test: test-part1 test-part2
