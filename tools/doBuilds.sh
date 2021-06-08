#!/usr/bin/env bash

declare CWD GITNAME GITROOT GITTAG ENVIRONMENTS BINLOC PIO
BINLOC="firmware"

get_pio() {
    echo -e "\nChecking PlatformIO environment."
    if grep -q "WSL" /proc/version; then
        # Running WSL
        PIO="/mnt/c/Users/$LOGNAME/.platformio/penv/Scripts/platformio.exe"
    elif grep -q "@WIN" /proc/version; then
        # Running Git Bash
        PIO="$HOME/.platformio/penv/Scripts/platformio.exe"
    else
        # Running some form of Linux
        PIO="$HOME/.platformio/penv/bin/platformio"
    fi
    if [[ ! -f "$PIO" ]]; then
        # PIO does not exist
        echo -e "\nERROR: Unable to find PlatformIO."
        exit
    fi
}

get_git() {
    if [[ ! -f $(which git 2>/dev/null) ]]; then
        echo -e "\nERROR: Git not found."
        exit
    fi
    echo -e "\nDetermining git root."
    if [[ ! $(git status 2>/dev/null) ]]; then
        echo -e "\nERROR: Git repository not found."
        exit
    fi
    GITROOT=$(git rev-parse --show-toplevel)
    GITNAME=$(git rev-parse --show-toplevel)
    GITNAME=${GITROOT##*/}
    GITTAG=$(git describe --tags --abbrev=0)
}

check_root() {
    CWD=$(pwd)
    if [[ ! -d "$GITROOT" ]]; then
        echo -e "\nERROR: Repository not found."
        exit
    fi
    cd "$GITROOT" || exit
}

get_envs() {
    echo -e "\nGathering build environments for $GITNAME."
    cd "$GITROOT" || exit
    readarray -t ENVIRONMENTS < <("$PIO" project data | grep "env_name" | cut -d'"' -f2)
}

list_envs() {
    echo -e "\nProcessing the following environments for $GITNAME:"
    for env in "${ENVIRONMENTS[@]}"
    do
        echo -e "\t$env"
    done
    sleep 3
}

create_version() {
    echo -e "\nCreating version JSON."
    sleep 1
    cat << EOF | tee "$GITROOT/data/version.json" "$GITROOT/$BINLOC/version.json" > /dev/null || exit
{
    "fw_version": "$GITTAG",
    "fs_version": "$GITTAG"
}
EOF
}

build_binaries() {
    cd "$GITROOT" || (echo -e "Environment not found." && exit)
    for env in "${ENVIRONMENTS[@]}"
    do
        echo -e "\nBuilding binaries for $env."
        sleep 3
        eval "$PIO" run -e "$env"
        echo -e "\nBuilding filesysyem for $env."
        sleep 3
        eval "$PIO" run --target buildfs -e "$env"
    done
}

copy_binaries() {
    echo
    if [ -d "$GITROOT"/"$BINLOC"/ ]; then
        for env in "${ENVIRONMENTS[@]}"
        do
            echo -e "Copying binaries for $env."
            cp "$GITROOT"/.pio/build/"$env"/firmware.bin "$GITROOT"/"$BINLOC"/"$env"_firmware.bin
            # cp "$GITROOT"/.pio/build/"$env"/partitions.bin "$GITROOT"/"$BINLOC"/"$env"_partitions.bin
            cp "$GITROOT"/.pio/build/"$env"/littlefs.bin "$GITROOT"/"$BINLOC"/"$env"_spiffs.bin
        done
    else
        echo -e "\nERROR: Unable to copy files to $GITROOT/$BINLOC"
    fi
}

pause() {
    read -p "Press any key to resume ..."
}

main() {
    get_pio "$@"
    get_git "$@"
    check_root "$@"
    get_envs "$@"
    list_envs "$@"
    #create_version "$@"
    build_binaries "$@"
    copy_binaries "$@"
    cd "$CWD" || exit
    echo -e "\nBuild and prep for $GITNAME complete.\n"
}

main "$@" && exit 0
