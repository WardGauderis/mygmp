#!/bin/sh
# eigen scriptje voor development

#cd scripts || exit


sudo ../click/userlevel/click glue.click &

sleep 1

sudo ../click/userlevel/click -p 10001 router.click &
sudo ../click/userlevel/click -p 10002 server.click &
sudo ../click/userlevel/click -p 10003 client21.click &
sudo ../click/userlevel/click -p 10004 client22.click &
sudo ../click/userlevel/click -p 10005 client31.click &
sudo ../click/userlevel/click -p 10006 client32.click &

wait
