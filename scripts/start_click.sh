#!/bin/sh
# eigen scriptje voor development

cd scripts || exit


sudo ../click/userlevel/click glue.click &

sleep 1

sudo ../click/userlevel/click -p 10001 router.click || echo segfault_router &
sudo ../click/userlevel/click -p 10002 server.click || echo segfault &
sudo ../click/userlevel/click -p 10003 client21.click || echo segfault &
sudo ../click/userlevel/click -p 10004 client22.click || echo segfault &
sudo ../click/userlevel/click -p 10005 client31.click || echo segfault &
sudo ../click/userlevel/click -p 10006 client32.click || echo segfault &

wait
