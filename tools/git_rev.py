#!/usr/bin/python

import subprocess
import os 

# Get Git project name
project = None
try:
    projcmd = "git rev-parse --show-toplevel"
    project = subprocess.check_output(projcmd, shell=True).decode().strip()
    project = project.split("/")
    project = project[len(project)-1]
except:
    project = os.path.dirname(os.path.realpath(__file__))
    project = project.split("/")
    project = project[len(project)-1]
if not project or project == "":
    project = "unknown"

# Get 0.0.0 version from latest Git tag
version = None
try:
    tagcmd = "git describe --tags --abbrev=0"
    version = subprocess.check_output(tagcmd, shell=True).decode().strip()
except:
    pass
if not version or version == "":
    version = "0.0.0"

# Get latest commit short from Git
commit = None
try:
    revcmd = "git log --pretty=format:'%h' -n 1"
    commit = subprocess.check_output(revcmd, shell=True).decode().strip()
except:
    pass
if not commit or commit == "":
    commit = "01234567"

# Get branch name from Git
branch = None
try:
    branchcmd = "git rev-parse --abbrev-ref HEAD"
    branch = subprocess.check_output(branchcmd, shell=True).decode().strip()
except:
    pass
if not branch or branch == "":
    branch = "devel"

# Make all available for use in the macros
print("-DPIO_SRC_NAM={0}".format(project))
print("-DPIO_SRC_TAG={0}".format(version))
print("-DPIO_SRC_REV={0}".format(commit))
print("-DPIO_SRC_BRH={0}".format(branch))
