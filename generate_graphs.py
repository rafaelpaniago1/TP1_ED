import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file into a DataFrame
df = pd.read_csv('sorting_times_unordered_long.csv')

# Group by Algorithm for plotting
grouped = df.groupby('Algorithm')

# Define a dictionary mapping algorithm codes to full names
algorithm_names = {
    'b': 'BubbleSort',
    'i': 'InsertionSort',
    's': 'SelectionSort',
    'm': 'MergeSort',
    'q': 'QuickSort',
    'h': 'ShellSort',
    'c': 'CountingSort',
    'bs': 'BucketSort',
    'r': 'RadixSort'
}

# Plotting
plt.figure(figsize=(12, 8))

for code, name in algorithm_names.items():
    algorithm_df = df[df['Algorithm'] == code]
    plt.plot(algorithm_df['Vector Size'], algorithm_df['Average Time'], marker='o', label=name)

plt.title('Sorting Algorithm Benchmarks (unordered and vector of long)')
plt.xlabel('Vector Size')
plt.ylabel('Average Time')
plt.legend()
plt.grid(True)
plt.show()

#####################################################

# Filter the DataFrame for vectors up to size 50
df_filtered = df[df['Vector Size'] <= 200]

# Plotting
plt.figure(figsize=(12, 8))

for code, name in algorithm_names.items():
    algorithm_df = df_filtered[df_filtered['Algorithm'] == code]
    plt.plot(algorithm_df['Vector Size'], algorithm_df['Average Time'], marker='o', label=name)

plt.title('Sorting Algorithm Benchmarks (unordered and vector of long)')
plt.xlabel('Vector Size')
plt.ylabel('Average Time')
plt.legend()
plt.grid(True)
plt.show()

