# **Memory-Manager**

## **Summary of the project:**

This is the Project for my Operating System class, written in pure C language.
It is a C library using the binary buddy memory management algorithm.
It is allocated with a large memory chunk in the beginning and later allocates 
smaller memory chunks in response to memory request. It needs to keep track of 
binary memory buddies and combine them to form larger memory chunk when both of
them are free.
It implemented the functions of grow memory and pre_grow memory which add extra
memory to the end or beginning of memory chunk. If there is insufficient memory
chunk at the end or beginning of current memory chunk, it has to migrate data to
another memory chunk of bigger size.

## **Requirement**

This program requires make and gcc to compile

## **Features**
* can call malloc just once but able to handle all later reasonable memory requests
* can keep track of binary memory buddies and combine them together when both of them are free
* can grows current memory chunk from either end depending on whether it's grow or pre_grow
* migrate current data to another memory chunk of bigger size if there is insufficient memory left for current memory chunk.