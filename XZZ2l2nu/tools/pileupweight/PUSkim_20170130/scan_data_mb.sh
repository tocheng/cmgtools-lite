#!/bin/sh


mbs="69200"
#mbs="60000 60500 61000 61500 62000 62500 63000 63500 64000 64500 65000 65500 66000 66500 67000 67500 68000 68500 69000 69500 70000 70500 71000 71500 72000"

#mbs="69200 68075 67921"
 
#mbs="63000 63100 63200 63300 63400 63500 63600 63700 63800 63900 64000 64100 64200 64300 64400 64500 64600 64700 64800 64900 65000 65100 65200 65300 65400 65500 65600 65700 65800 65900 66000 66100 66200 66300 66400 66500 66600 66700 66800 66900 67000 67100 67200 67300 67400 67500 67600 67700 67800 67900 68000"
#mbs="62194 61674 62118 61658 "
#mbs="68000 68100 68200 68300 68400 68500 68600 68700 68800 68900 69000 69100 69200 69300 69400 69500 69600 69700 69800 69900 70000 70100 70200 70300 70400 70500 70600 70700 70800 70900 71000"
#mbs="62154"
#mbs="61665"
#mbs="68715 "
#mbs="96000 97000 98000 99000 100000 101000 102000 103000 104000 105000 106000 107000 108000 109000 110000 111000 112000 113000 114000 115000 116000 117000 118000 119000 120000 121000 122000 123000 124000 125000 126000 127000 128000 129000 130000 131000 132000 133000 134000 135000 136000 137000 138000 139000 140000 141000 142000 143000 144000 145000 146000 147000 148000 149000 150000 "
#mbs="78000 78500 79000 79500 80000 80500 81000 81500 82000 82500 83000 83500 84000 84500 85000 85500 86000 86500 87000 87500 88000 88500 89000 89500 90000 90500 91000 91500 92000 92500 93000 93500 94000 94500 95000 "
#mbs="61000 61100 61200 61300 61400 61500 61600 61700 61800 61900 62000 62100 62200 62300 62400 62500 62600 62700 62800 62900 63000 "
#mbs="50000 50500 51000 51500 52000 52500 53000 53500 54000 54500 55000 55500 56000 56500 57000"
#mbs=" 61100 61200 61300 61400  61600 61700 61800 61900  62100 62200 62300 62400  62600 62700 62800 62900  "
#mbs="$mbs 57500 58000 58500 59000 59500 60000 60500 61000 61500 62000 62500 63000 63500 64000 64500 65000 65500 66000 66500 67000 67500 68000 68500 69000 69500 70000 70500 71000 71500 72000 72500 73000 73500 74000 74500 75000 75500 76000 76500 77000 77500 " 
#mbs="69100 69200 69300 69400 69500 69600 69700 69800 69900 70000 70100 70200 70300 70400 70500"

out="results"
mkdir -p $out

goldenJson="Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt"
pileupJson="pileup_latest_20161206.txt"

dtTag="pileup_DATA_80x_2016FullReReco_"
mcTag="pileup_MC_80x_2016FullReReco_"

mkdir -p $out

for mb in $mbs;
do
  echo "Scanning mb xsec = $mb ub :"
  echo "  - calculating data pileup profile ... "
  pileupCalc.py -i \
    ${goldenJson} \
    --inputLumiJSON \
    ${pileupJson} \
    --calcMode true  \
    --minBiasXsec ${mb} \
    --maxPileupBin 100 \
    --numPileupBins 100 \
    ${out}/${dtTag}${mb}.root &>  ${out}/${dtTag}${mb}.log &

done






