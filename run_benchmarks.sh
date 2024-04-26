#!/bin/bash

# Define algorithms
algorithms=("b" "i" "s" "m" "q" "h" "c" "bs" "r")

# Define vector sizes from 10 to 1000, varying by 10
vector_sizes=$(seq 10 50 1010)

# Define number of seeds
num_seeds=1

# Output CSV file
output_file="sorting_times_unordered_long.csv"

# Create header in CSV file
echo "Algorithm,Vector Size,Average Time" > "$output_file"

# Loop through each algorithm
for alg in "${algorithms[@]}"; do
    echo "Algorithm: $alg"
    # Loop through each vector size
    for size in $vector_sizes; do
        echo "Vector Size: $size"
        total_time=0
        # Initialize array to store times for each seed
        declare -a times
        # Loop through each seed
        for seed in $(seq 1 $num_seeds); do
            # Run the sorting algorithm benchmarking tool and save time
            time_taken=$(./sortperf -z $size -s $seed -a $alg | grep -oP '(?<=time )[0-9]+.[0-9]+')
            times+=($time_taken)
            total_time=$(awk "BEGIN {print $total_time + $time_taken}")
        done
        # Calculate average time
        average_time=$(awk "BEGIN {print $total_time / $num_seeds}")
        # Append results to CSV file
        echo "$alg,$size,$average_time" >> "$output_file"
    done
done

echo "Results saved to $output_file"