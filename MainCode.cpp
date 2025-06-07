#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <time.h>






/*
-------------------------1 HeapNode
Part 1 - Data Structures 2 Heap
-------------------------3 HuffmanTree
-------------------------4 Huffstructor
*/


// [1]
class HeapNode
{
public:
    int byte;
    int frequency;
    HeapNode* left;
    HeapNode* right;

    HeapNode(int b = 0, int f = 0)
    {
        byte = b;
        frequency = f;
        left = nullptr;
        right = nullptr;
    }
    // [2]
    bool operator>(HeapNode& node)
    { //whenver the operator'>' is called it checks if the freq of current node is smaller
        return frequency > node.frequency;
    }
    bool operator<(HeapNode& node)
    { //whenver the operator '<' is called it checks if the freq of current node is bigger
        return frequency < node.frequency;
    }
};

// [1]
class Heap
{
public:
    HeapNode* arr;
    int size;
    int capacity;

    Heap()
    {
        capacity = 256;
        size = 0;
        arr = new HeapNode[capacity];
    }
    int getLeftChildIndex(int parentIndex) {
        return 2 * parentIndex + 1;
    }
    int getRightChildIndex(int parentIndex) {
        return 2 * parentIndex + 2;
    }
    int getParentIndex(int childIndex) {
        return (childIndex - 1) / 2;
    }
    bool hasLeftChild(int parentIndex) {
        if (getLeftChildIndex(parentIndex) < size)
        {
            return true;
        }
        return false;
    }
    bool hasRightChild(int parentIndex) {
        if (getRightChildIndex(parentIndex) < size)
        {
            return true;
        }
        return false;
    }
    bool hasParent(int childIndex) {
        if (getParentIndex(childIndex) >= 0)
        {
            return true;
        }
        return false;
    }
    void swap(int i1, int i2)
    {
        HeapNode temp = arr[i1];
        arr[i1] = arr[i2];
        arr[i2] = temp;
    }
    void ensureHeapCapacity()
    {
        if (size < capacity)
        {
            return;
        }
        HeapNode* newArr = new (std::nothrow) HeapNode[capacity * 2];
        if (newArr != NULL)
        {
            capacity *= 2;
            for (int i = 0; i < size; i++)
                newArr[i] = arr[i];

            delete[] arr;
            arr = newArr;
        }
    }
    void heapifyUp()
    {
        int index = size - 1;
        while (hasParent(index) && arr[getParentIndex(index)] > arr[index])
        {
            swap(getParentIndex(index), index);
            index = getParentIndex(index);
        }
    }
    void heapifyDown() {
        int index = 0;
        while (hasLeftChild(index))
        {
            int smallestChildIndex = getLeftChildIndex(index);
            if (hasRightChild(index) && arr[getRightChildIndex(index)] < arr[smallestChildIndex])
            {
                smallestChildIndex = getRightChildIndex(index);
            }
            if ((arr[index] < arr[smallestChildIndex]))
            {
                break;
            }
            else
            {
                swap(index, smallestChildIndex);
                index = smallestChildIndex;
            }
        }
    }
    bool poll(HeapNode* item = NULL)
    {
        if (size == 0)
            return false;

        if (item != NULL)
            *item = arr[0];

        arr[0] = arr[size - 1];
        size--;
        heapifyDown();
        return true;
    }
    void add(int byte, int frequency) // add for the byte, freq
    {
        ensureHeapCapacity();
        arr[size] = HeapNode(byte, frequency);
        size++;
        heapifyUp();
    }
    void add(const HeapNode& node) // add for the pointer merged
    {
        ensureHeapCapacity();
        arr[size] = node;
        size++;
        heapifyUp();
    }
    int getSize()
    {
        return size;
    }
};


// [2]
class HuffmanTree
{
public:
    HeapNode* root;
    //Code Storage:
    //table to store the codes for each byte
    //Stores the binary codes (as strings) for each possible byte (0-255)
    char codes[256][256];

    //Temporary code buffer used during generation.
    //for traversal (for each character), once leaf node is reached it copies the code into the 2d array
    char code[256];

    HuffmanTree()
    {
        root = nullptr;
        memset(codes, 0, sizeof(codes));  // Initialize all codes to 0
        memset(code, 0, sizeof(code));    // Initialize code buffer to 0
    }

    // [4] This function was taken from geeks for geeks and modified to our
    // code since we dont use any external libraries
    // https://www.geeksforgeeks.org/huffman-coding-in-cpp/
    //https://www.programiz.com/dsa/huffman-coding
    void generateBinaryCodes(HeapNode* root, char code[], int depth)
    {
        if (root == 0)
            return;
        if (root->left == 0 && root->right == 0) {
            code[depth] = '\0';
            strcpy(codes[root->byte], code);
            return;
        }
        if (root->left != 0) {
            code[depth] = '0';
            generateBinaryCodes(root->left, code, depth + 1);
        }
        if (root->right != 0) {
            code[depth] = '1';
            generateBinaryCodes(root->right, code, depth + 1);
        }
    }
};

// [5]
class HuffStructor {
public:
    Heap* heap;
    HuffmanTree* huffmanTree;
    int frequency[256];

    HuffStructor() {
        heap = new Heap();
        huffmanTree = new HuffmanTree();
        memset(frequency, 0, sizeof(frequency)); //initialize frequency array to 0   // [2]
    }
    ~HuffStructor() {
        delete heap;
        delete huffmanTree;
    }
};



/*
-------------------------1 BitWriter
Part 2 - Bits Managment |
-------------------------2 BitReader
*/


// [3] The structure of this class was taken from ai and we made edits on it to work in our code 
class BitWriter
{
    FILE* file;
    unsigned char buffer; // 8-bit buffer to collect bits
    int bitCount; // How many bits are currently in the buffer
public:
    BitWriter(FILE* f)
    {
        file = f;
        buffer = 0;
        bitCount = 0;
    }
    void writeBit(int bit)
    {
        buffer = buffer << 1;
        if (bit) buffer |= 1;
        bitCount++;
        if (bitCount == 8)
            flush();
    }
    // write the entire code string for exampkel 1010101001000000110101
    void writeCode(const char* code)
    {
        for (int i = 0; code[i] != '\0'; i++)
        {
            int bit = code[i] - '0';  // Convert string 0 or 1 to integer  0 or 1
            writeBit(bit);
        }
    }
    void flush()
    {
        if (bitCount > 0)
        {
            buffer <<= (8 - bitCount);  // add some zeroes to make a full byte (pad)
            fputc(buffer, file);        // then write byte tofile
            buffer = 0;
            bitCount = 0;
        }
    }
    ~BitWriter() {
        flush();
    }
};

// [3] -> [2]
class BitReader
{
    FILE* file;
    unsigned char* buffer;
    int buffer_pos;
    int buffer_size;
    unsigned char current_byte;
    int bit_count;
public:
    BitReader(FILE* f, int buf_size)
    {
        file = f;
        buffer_size = buf_size;
        buffer = new unsigned char[buffer_size]; // Allocate memory to read data from file
        buffer_pos = 0;
        current_byte = 0;
        bit_count = 0;
        // Pre-fill buffer
        refillBuffer(); //Reads the first chunk of data from the file into the buffer
    }
    ~BitReader() {
        delete[] buffer;
    }
    void refillBuffer()
    {
        buffer_size = fread(buffer, 1, this->buffer_size, file); //reads up to  buffer size bytes from the file into the buffer
        buffer_pos = 0;
    }
    int readBit()
    {
        if (bit_count == 0) //if no bits left in the current byte
        {
            if (buffer_pos >= buffer_size) //if we reached the end of the buffer
            {
                refillBuffer();
                if (buffer_size == 0)
                    return -1; // end of file
            }
            current_byte = buffer[buffer_pos++]; //load next byte from the buffer
            bit_count = 8; //each byte is 8 bits
        }
        //AI[2]
        int position = bit_count - 1;         // Where the bit is inside the byte
        int shifted = current_byte >> position; // Move that bit to the rightmost position
        int bit = shifted & 1;                 // Mask all other bits, keep only the last one
        bit_count--;                           // Update count
        return bit;
    }
};


/*
--------------------------------------------1 encodeTree
Part 3 - Saving and Reconstructing the Tree 2 decodeTree
--------------------------------------------3 deleteTree
*/


// [2]
void encodeTree(FILE* out, HeapNode* final)
{
    if (final == 0)
        return;
    if (final->left == 0 && final->right == 0)
    {
        fputc('L', out); // write L to the file to tell a leaf node is here
        fputc(final->byte, out);
    }
    else
    {
        fputc('I', out); // write I to the file to tell a internal node is here
        encodeTree(out, final->left);
        encodeTree(out, final->right);
    }
}

// [3] -> [2]
HeapNode* decodeTree(FILE* input)
{
    int type = fgetc(input);
    if (type == 'L')
    {
        int byte = fgetc(input);
        return new HeapNode(byte, 0);
    }
    else if (type == 'I')
    {
        HeapNode* node = new HeapNode(-1, 0);
        node->left = decodeTree(input);
        node->right = decodeTree(input);
        return node;
    }
    return nullptr;
}


void deleteTree(HeapNode* node)
{
    if (node == 0)
        return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}



/*
---------------------------------------1 compressFile
Part 3 - Compression and Decompression |
---------------------------------------2 decompressFile
*/



void compressFile(const char* input_file, const char* output_file, int buffer_size)
{
    clock_t start = clock(); // This was inpired from the Applied programming project

    FILE* input = fopen(input_file, "rb"); // taken from applied programming (files)
    if (input == 0)
    {
        printf("Failed to open %s\n", input_file);
        return;
    }

    // [6]
    fseek(input, 0, SEEK_END); //moves the file pointer to the end of the file
    long long file_size = ftell(input); //current position of the pointer
    rewind(input); //moves the pointer back to the beginning of the file

    // [2]
    unsigned char* buffer = new unsigned char[buffer_size];
    // we used unsigned because we only need positive values so we get
    // from 0 to 255 not from -128 to 127 ( we only use +ve values for bytes)

    if (buffer == 0)
    {
        printf("Memory allocation failed.\n");
        fclose(input);
        return;
    }
    printf("Processing %s (Size: %lld bytes) with buffer size: %d\n", input_file, file_size, buffer_size);

    // [5]
    long long total_read = 0;
    int frequency[256] = { 0 };

    // [3] Buffering idea as a whole was understood from ai
    while (total_read < file_size) // loop till we read the whole file
    {

        /*All variables :
        * file_size: totall no of bytes
        * total_read: bytes we have read
        * to_read: bytes we need to read
        * buffer_size: bytes read in one time ( we took it from CLI )
        * bytes_read: bytes read in this iteration
        */
        int to_read;
        if ((file_size - total_read) < buffer_size)
        {
            to_read = file_size - total_read;
        }
        else
        {
            to_read = buffer_size;
        }

        int bytes_read = fread(buffer, 1, to_read, input);
        // read a no of 1* (to_read bytes) from input to buffer
        //sees how many bytes are actually read.

        if (bytes_read != to_read)
        {
            printf("Error reading file at position %lld\n", total_read);
            break;
        }

        total_read += bytes_read;

        // [5]
        for (int i = 0; i < bytes_read; i++)
        {
            frequency[buffer[i]]++;
        }
        printf("Processed: %lld/%lld bytes (%.1f%%)\r", total_read, file_size, (float)total_read / file_size * 100);
        fflush(stdout);
    }

    // [5]
    HuffStructor s;
    for (int i = 0; i < 256; ++i)
    {
        if (frequency[i] > 0)
        {
            s.heap->add(i, frequency[i]);
        }
    }

    // [3]
    //once only one node remains it becomes the final Huffman tree root
    while (s.heap->getSize() > 1)
    {
        HeapNode* left = new HeapNode();
        HeapNode* right = new HeapNode();
        s.heap->poll(left); //store the smallest freq node in left
        s.heap->poll(right); //store the next smallest node in right


        // [2]
        HeapNode* merged = new HeapNode(-1, left->frequency + right->frequency);
        merged->left = left;
        merged->right = right;
        s.heap->add(*merged);
    }

    // [5]
    HeapNode* finalRoot = new HeapNode();
    s.heap->poll(finalRoot); // get the root of the tree
    s.huffmanTree->root = finalRoot; //make the root of the tree is the root of the heap
    s.huffmanTree->generateBinaryCodes(finalRoot, s.huffmanTree->code, 0);

    FILE* output = fopen(output_file, "wb");
    if (output == 0)
    {
        printf("Failed to open output file: %s\n", output_file);
        return;
    }

    // [3]
    const char MAGIC[] = "HUFF";
    fwrite(MAGIC, 1, 4, output);
    fwrite(&file_size, sizeof(long long), 1, output);

   
    encodeTree(output, finalRoot);
    BitWriter writer(output);
    rewind(input);

    total_read = 0;

    // [3]
    
    while (total_read < file_size)
    {
        int to_read = file_size - total_read;

        if (to_read > buffer_size)
            to_read = buffer_size;

        // read from input file into buffer
        int bytes_read = fread(buffer, 1, to_read, input);
        if (bytes_read <= 0)
            break;

        // for each byte write its Huffman code
        for (int i = 0; i < bytes_read; i++)
        {
            const char* code = s.huffmanTree->codes[buffer[i]];
            writer.writeCode(code);
        }

        total_read += bytes_read;

        // [2]
        double progress = (double)total_read / (double)file_size;
        double percent = progress * 100.0;

        double elapsed_sec = (double)(clock() - start) / CLOCKS_PER_SEC;

        double eta_sec;
        if (progress > 0.0)
        {
            eta_sec = (elapsed_sec * (1.0 - progress)) / progress;
        }
        else
        {
            eta_sec = 0.0;
        }

        printf("Compressed: %lld/%lld bytes (%.1f%%)  ETA: %.1f s\r", total_read, file_size, percent, eta_sec);
        fflush(stdout);
    }


    writer.flush();
    fseek(output, 0, SEEK_END);
    long long compressed_size = ftell(output);
    printf("\nCompression complete. Original: %lld bytes, Compressed: %lld bytes (%.1f%%)\n", file_size, compressed_size, (float)compressed_size / file_size * 100);

    deleteTree(finalRoot);

    delete[] buffer;


    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTime: %.2f seconds\n", time_spent);
    return;
}


// [5,6,3]
void decompressFile(const char* input_file, const char* output_file, int buffer_size)
{
    clock_t start = clock();

    FILE* input = fopen(input_file, "rb"); //read binary mode
    if (!input) //failed to open file
    {
        printf("Failed to open input file: %s\n", input_file);
        return;
    }

    FILE* output = fopen(output_file, "wb"); //write binary mode
    if (!output)
    {
        fclose(input);
        printf("Failed to open output file: %s\n", output_file);
        return;
    }

    // Verify magic number
    char magic[5];
    if (fread(magic, 1, 4, input) != 4) //reads first 4 bytes in the file
    {
        fclose(input);
        fclose(output);
        printf("Error reading magic number\n");
        return;
    }
    magic[4] = '\0';

    if (strcmp(magic, "HUFF") != 0) //if the magic number is not huff
    {
        fclose(input);
        fclose(output);
        printf("Invalid Huffman compressed file\n");
        return;
    }

    // [3]
    long long original_size; //to store the original size of the file
    if (fread(&original_size, sizeof(long long), 1, input) != 1)  //reads the original size of the file
    {
        fclose(input);
        fclose(output);
        printf("Error reading original size\n");
        return;
    }

    HeapNode* root = decodeTree(input);
    if (root == NULL) //checks if decoding failed
    {
        fclose(input);
        fclose(output);
        printf("Failed to decode Huffman tree\n");
        return;
    }

    HuffmanTree huffmanTree;
    huffmanTree.root = root;
    huffmanTree.generateBinaryCodes(root, huffmanTree.code, 0); //generated binary codes for the tree

    BitReader reader(input, buffer_size);
    long long bytes_written = 0;

    // [2,4]
    //https://www.w3schools.com/dsa/dsa_ref_huffman_coding.php
    unsigned char* out_buffer = new unsigned char[buffer_size]; //allocates memory for the output buffer
    int buffer_pos = 0; //track current position in the output buffer

    HeapNode* current = root; //tree traversal
    while (bytes_written < original_size && current != nullptr) //keep decoding until original size is met
    {
        int bit = reader.readBit(); //read one bit from the compressed file
        if (bit < 0)
            break;  //  end of the file

        if (bit == 1) {
            current = current->right;
        }
        else {
            current = current->left;
        }

        if (current != NULL && current->left == NULL && current->right == NULL)
        {
            out_buffer[buffer_pos++] = current->byte; //if leaf node reached write the byte to the output buffer
            bytes_written++;
            current = root; //reset current to root for next byte

            if (buffer_pos >= buffer_size) { //full buffer
                fwrite(out_buffer, 1, buffer_pos, output);
                buffer_pos = 0;
            }

            if (bytes_written % 1024 * 1024 == 0)
            {
                double progress = (double)bytes_written / (double)original_size;
                double percent = progress * 100.0;

                double elapsed_sec = (double)(clock() - start) / CLOCKS_PER_SEC;

                double eta_sec;
                if (progress > 0.0)
                {
                    eta_sec = (elapsed_sec * (1.0 - progress)) / progress;
                }
                else
                {
                    eta_sec = 0.0;
                }

                printf("Decompressed: %lld/%lld bytes (%.1f%%)  ETA: %.1f s\r",
                    bytes_written, original_size,
                    percent,
                    eta_sec);
                fflush(stdout);
            }
        }
    }
    if (buffer_pos > 0)
        fwrite(out_buffer, 1, buffer_pos, output); //write remaning bytes in the buffer

    deleteTree(root);
    fclose(input);
    fclose(output);

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nDecompression completed. Decompressed: %lld bytes in %.2f seconds\n", bytes_written, time_spent);

    return;
}



/*
------------ o
Part 4 - Main }
------------ o
*/


// [5]
int main(int argc, char* argv[])
{
    // Default values
    int compress = 0;
    int decompress = 0;
    char* input_file = NULL;
    char* output_file = NULL;
    int buffer_size = 8192;

    // Parsing command line arguments was explained in lecture
    // we wrote the code and ai suggested some errors handling
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            if (decompress == 1) {
                printf("Error: cannot use both -c and -d\n");
                return 1;
            }
            compress = 1;
            if (i + 2 >= argc) {
                printf("Error:  missing filenames \n");
                return 1;
            }
            input_file = argv[i + 1];
            output_file = argv[i + 2];
            i += 2;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            if (compress == 1) {
                printf("Error: cannot use -c and -d together\n");
                return 1;
            }
            decompress = 1;
            if (i + 2 >= argc) {
                printf("Error: missing filenames\n");
                return 1;
            }
            input_file = argv[i + 1];
            output_file = argv[i + 2];
            i += 2;
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
            if (i + 1 >= argc) {
                printf("Error: missing buffer size \n");
                return 1;
            }

            buffer_size = atoi(argv[i + 1]);// atoi converts string to integer // [3]
            if (buffer_size <= 0)
            {
                printf("Error: Invalid buffer size\n");
                return 1;
            }
            i++;
        }
        else {
            printf("Error: unknown argument %s\n", argv[i]);
            return 1;
        }
    }

    if (compress != 1 && decompress != 1) {
        printf("Error: You shoukd use -c or -d\n");
        return 1;
    }

    if (compress == 1)
        compressFile(input_file, output_file, buffer_size);


    else if (decompress == 1)
        decompressFile(input_file, output_file, buffer_size);

    return 0;
}

/*
    At the end of this project we would like to thank Dr nour and the department for pushing us to learn
    and practise advanced projects compared to the projects other univerties students work on. Thank you :)
*/
