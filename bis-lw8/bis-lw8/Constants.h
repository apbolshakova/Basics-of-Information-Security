#define SRC_FILE_PATH "src.bmp"
#define DEST_FILE_PATH "result.bmp"
#define TEXT_FILE_PATH "text.txt"
#define DECODED_FILE_PATH "decoded.txt"

#define OFFSET_POS_HEX 0x0A //адрес отступа, начиная с которого идут байты изображения
#define PALETTE_POS_HEX 0x1C //адрес палитры
#define SIZE_POS_HEX 0x22 //адрес размера контейнера

#define PALETTE 24
#define MIN_PACK 1
#define MAX_PACK 7
#define BITS_IN_BYTE 8
#define LEN_NUMBER_SIZE 8 //количество пар бит для хранения длины зашифрованного текста
#define PACK_SIZE 3 //количество бит для хранения степени упаковки