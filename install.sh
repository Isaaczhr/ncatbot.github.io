#!/bin/bash

install_nbpe() {
    local url="https://plugins.ncatbot.xyz/cli/nbpe"
    local dest="/bin/nbpe"

    # Download the file
    if curl -L -o "$dest" "$url"; then
        # Make it executable
        chmod +x "$dest"
        echo "NcatBot Plugins Edition CLI installed successfully."
        echo "Use 'nbpe help' for usage."
    else
        echo "Failed to download nbpe from $url"
        exit 1
    fi
}

install_nbpe