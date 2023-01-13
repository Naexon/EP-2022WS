for i in {0..18}
do
    #echo $i
    ./bench_csv.sh -p version_$i -n 10000000000000 -r 5
done