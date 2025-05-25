#! /bin/bash

apt install curl -y

chmod +x send_ip.sh

sudo mv send-ip.service /etc/systemd/system/

systemctl daemon-reload
systemctl enable send-ip.service
systemctl start send-ip.service
