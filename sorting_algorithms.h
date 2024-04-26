#include <stdio.h>
#ifndef SORTING_ALGORITHMS
#define SORTING_ALGORITHMS


void bubbleSort(int arr[], int n);

void insertionSort(int arr[], int n);

void selectionSort(int arr[], int n);

void merge(int arr[], int l, int m, int r);

void mergeSort(int arr[], int l, int r);

int partition(int arr[], int low, int high);

void quickSort(int arr[], int low, int high);

void shellSort(int arr[], int n);

void countingSort(int arr[], int n, int exp);

void radixSort(int arr[], int n);

void bucketSort(int arr[], int n);

#endif