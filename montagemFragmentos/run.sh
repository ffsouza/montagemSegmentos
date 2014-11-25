
export INPUT=../../geracaoSegmentos/output
export OUTPUT=./result

export EXECUTABLE=./Debug/montagemFragmentos

rm $OUTPUT -rf
mkdir $OUTPUT

$EXECUTABLE -i $INPUT/n500_t50.dat  -o $OUTPUT/n15_t50_d5.out -ol 5 -e 0.0
$EXECUTABLE -i $INPUT/n500_t50.dat  -o $OUTPUT/n500_t50.out -ol 5 -e 0.0
$EXECUTABLE -i $INPUT/n100_t50_d5.dat -o $OUTPUT/n100_t50_d5.ol3_e0.out -ol 3 -e 0.0
$EXECUTABLE -i $INPUT/n100_t50_d5.dat -o $OUTPUT/n100_t50_d5.ol5_e0.out -ol 5 -e 0.0
$EXECUTABLE -i $INPUT/n1000_t50.dat  -o $OUTPUT/n1000_t50.out -ol 3 -e 0.0
$EXECUTABLE -i $INPUT/n1000_t50_d5.dat -o $OUTPUT/n1000_t50_d5.out -ol 2 -e 0.0
$EXECUTABLE -i $INPUT/n100_t100_d15.dat  -o  $OUTPUT/n100_t100_d15.out -ol 5 -e 0.0
$EXECUTABLE -i $INPUT/n100_t100_d15.dat  -o $OUTPUT/n100_t100_d15.out -ol 5 -e 0.0
$EXECUTABLE -i $INPUT/n150_t100_d15.dat  -o $OUTPUT/n150_t100_d15.out -ol 5 -e 0.0
$EXECUTABLE -i $INPUT/n150_t100_d15.dat  -o $OUTPUT/n150_t100_d15.out -ol 5 -e 0.0
$EXECUTABLE -i $INPUT/n2000_t100_d15.dat  -o $OUTPUT/n2000_t100_d15.out -ol 5 -e 0.0
$EXECUTABLE -i $INPUT/n3000_t50_d15.dat  -o $OUTPUT/n3000_t50_d15.out -ol 4 -e 0.0
$EXECUTABLE -i $INPUT/n5000_t25_d5.dat  -o $OUTPUT/n5000_t25_d5.out -ol 10 -e 0.0
$EXECUTABLE -i $INPUT/n100000_t25_d7.dat  -o $OUTPUT/n100000_t25_d7.out -ol 5 -e 0.0
