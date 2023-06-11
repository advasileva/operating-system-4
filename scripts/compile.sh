#!/bin/bash

gcc ./4-5-points/client-buyers.c -lpthread -o ./4-5-points/client-buyers
gcc ./4-5-points/client-seller.c -lpthread -o ./4-5-points/client-seller
gcc ./4-5-points/server.c -lpthread -o ./4-5-points/server
gcc ./6-7-points/client-buyers.c -lpthread -o ./6-7-points/client-buyers
gcc ./6-7-points/client-seller.c -lpthread -o ./6-7-points/client-seller
gcc ./6-7-points/server.c -lpthread -o ./6-7-points/server
gcc ./6-7-points/monitoring.c -lpthread -o ./6-7-points/monitoring
echo "Programs compiled"
