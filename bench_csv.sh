# define vars
mem_limit=102400
rama="ramasort"

# get scriptname for usage msg
script_name=`basename "$0"`

# only accept call with 6 args (including flags)
if [ $# -ne 6 ]
then
    echo "Usage: ${script_name} -p program_dir -n N -r repeats" >&2
    exit 1
fi

# parse call
while getopts p:n:r: flag
do
    case "${flag}" in
        p)  program_dir=${OPTARG};;
        n)  n=${OPTARG};;
        r)  repeats=${OPTARG};;
        ?)  echo "Usage: ${script_name} -p program_dir -n N -r repeats" >&2
            exit 1;;
    esac
done

# check file
if [ ! -f "./$program_dir/$rama" ];
then
    echo "Target not found! Please make sure that the program directory contains the $rama executable." >&2
    exit 1
fi

# define perf command
perf_command="ulimit -S -v $mem_limit; perf stat -x , -e cycles -e instructions -e branch-misses -e LLC-load-misses -e LLC-store-misses -e duration_time ./$program_dir/$rama $n"

printf "\nStarting benchmark of ./$program_dir/$rama with memory limit of $mem_limit KB:\n\n"

# setup bench dir
mkdir $program_dir/bench

# first iteration
echo -n "Running iteration 0 ... "
eval "$perf_command 1> $program_dir/bench/output 2> $program_dir/bench/stats.csv"
echo "done"

# loop iterations
for (( i=1; i<$(($repeats)); i++))
    do
        echo -n "Running iteration $i ... "
        eval "$perf_command 1> /dev/null 2>> $program_dir/bench/stats.csv"
        echo "done"
done

# output of first iteration
printf "\nProgram output of first iteration: \n\n"
cat $program_dir/bench/output

printf "\nProgram output and stats can be found in the ./$program_dir/bench/ directory.\n\n"