#! /bin/bash

sudo apt install curl -y

sudochmod +x send_ip.sh

sudo mv send-ip.service /etc/systemd/system/

sudo systemctl daemon-reload
sudo systemctl enable send-ip.service
sudo systemctl start send-ip.service
