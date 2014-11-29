#
#
#
#

export OUTPUT=./output
rm $OUTPUT -rf

mkdir $OUTPUT
export OUTPUT=./output
./Debug/geracaoSegmentos -o $OUTPUT/n15_t50_d5.dat -n 15 -t 50 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n500_t15_d5.dat -n 500 -t 15 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n1000_t10_d5.dat -n 1000 -t 10 -d 4
./Debug/geracaoSegmentos -o $OUTPUT/n1000_t20_d7.dat -n 1000 -t 20 -d 7
./Debug/geracaoSegmentos -o $OUTPUT/n1000_t15_d5.dat -n 1000 -t 15 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n1000_t20_d5.dat -n 1000 -t 20 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n1000_t35_d4.dat -n 2000 -t 35 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n2000_t15_d5.dat -n 2000 -t 15 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n2000_t25_d4.dat -n 2000 -t 25 -d 4
./Debug/geracaoSegmentos -o $OUTPUT/n2000_t25_d5.dat -n 2000 -t 25 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n3000_t30_d5.dat -n 3000 -t 30 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n5000_t25_d5.dat  -n 5000 -t 25 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n10000_t25_d4.dat -n 10000 -t 20 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n100000_t25_d7.dat  -n 100000 -t 25 -d 7

./Debug/geracaoSegmentos -o $OUTPUT/n500_t50.dat  -n 500 -t 50
./Debug/geracaoSegmentos -o $OUTPUT/n100_t50_d5.dat  -n 100 -t 50 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n1000_t50.dat  -n 1000 -t 50
./Debug/geracaoSegmentos -o $OUTPUT/n1000_t50_d5.dat  -n 1000 -t 50 -d 5
./Debug/geracaoSegmentos -o $OUTPUT/n100_t100_d15.dat  -n 100 -t 50 -d 15
./Debug/geracaoSegmentos -o $OUTPUT/n100_t100_d15.dat  -n 100 -t 50 -d 15
./Debug/geracaoSegmentos -o $OUTPUT/n3000_t50_d15.dat  -n 3000 -t 50 -d 25
./Debug/geracaoSegmentos -o $OUTPUT/n150_t100_d15.dat  -n 150 -t 100 -d 15
./Debug/geracaoSegmentos -o $OUTPUT/n150_t100_d15.dat  -n 150 -t 100 -d 15
./Debug/geracaoSegmentos -o $OUTPUT/n2000_t100_d15.dat  -n 2000 -t 100 -d 25


