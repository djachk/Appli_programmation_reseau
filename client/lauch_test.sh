#!/usr/bin/bash
make run
cd ../serveur
make

xterm -title 'server' -geometry '88x23+0+0' -e ' java Serveur 8888 9000 ;bash' &
cd ../client
#xterm -title 'serverTCP' -geometry '88x22+0+0' -e ' ncat -l 127.0.0.1 9999 ;bash' &
#xterm -title 'send_UDP' -geometry '88x22+1000+0' -e ' ncat -u 127.0.0.1 12321 ; bash' &
sleep 1
xterm -title 'client' -geometry '88x22+0+800' -e ' ./run 3333 ;bash' &
#xterm -title 'client' -geometry '88x22+0+800' -e ' gdb run ;bash' &
xterm -title 'client' -geometry '88x22+1000+800' -e ' ./run 3334 ;bash' &
cd ../promoteur
make
xterm -title 'promoteur' -geometry '88x11+1000+0' -e ' ./promoteur 127.0.0.1 9000 225.192.192.192 9001 ; bash' &
xterm -title 'promoteur' -geometry '88x11+1000+220' -e ' ./promoteur 127.0.0.1 9000 226.192.192.192 9002 ; bash' &

#ncat -vlu 225.192.192.192 9999
