
INPUT_FILE=$1
OUTPUT_NAME=$2
NUMBER_OF_NEURONS=$3

mkdir workspace
cd workspace

cp $INPUT_FILE .

git clone https://github.com/joaoVictorPinto/TrigCaloRingerAnalysisPackages.git

mv TrigCaloRingerAnalysisPackages/root/FastNetTool .
mv TrigCaloRingerAnalysisPackages/root/buildthis.sh .
rm -rf TrigCaloRinger*

source buildthis.sh
rc compile
cp FastNetTool/scripts/grid_exec.py .

python grid_exec.py --input=$INPUT_FILE --output=$OUTPUT_NAME.n$NUMBER_OF_NEURONS.save --neuron=$NUMBER_OF_NEURONS



scp -o "StrictHostKeyChecking=no" $OUTPUT_NAME.N$NUMBER_OF_NEURONS.save jodafons@lxplus.cern.ch:/tmp/jodafons/public/


