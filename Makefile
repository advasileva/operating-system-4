tests = 3
# dir=4-5-points
dir=6-7-points

chmod:
	chmod -R +xr .

.PHONY: all test clean
compile:
	./scripts/compile.sh

run:
	make chmod
	./scripts/run.sh ${tests}

server:
	./${dir}/server 50124 127.0.0.1 50225 50226 127.0.0.1 50321

buyers:
	./${dir}/client-buyers 127.0.0.1 50124 tests/test2.in 127.0.0.1 50321

seller-1:
	./${dir}/client-seller 50225 127.0.0.1 50321

seller-2:
	./${dir}/client-seller 50226 127.0.0.1 50321

monitoring:
	./${dir}/monitoring 50321
