#define SRC_FILE_PATH "src.bmp"
#define DEST_FILE_PATH "result.bmp"
#define TEXT_FILE_PATH "text.txt"
#define DECODED_FILE_PATH "decoded.txt"

#define BIN_BASE 2
#define BITS_IN_BYTE 8

#define OFFSET_POS_HEX 0x0A //����� �������, ������� � �������� ���� ����� �����������
#define PALETTE_POS_HEX 0x1C //����� �������
#define SIZE_POS_HEX 0x22 //����� ������� ����������

#define PALETTE 24 //������ ������� ��� �������� ������������ �����
#define FILE_SIZE_LEN 4 //���������� ����, ���������� �� ������ �����������
#define LEN_SIZE 16 //���������� ��� ��� �������� ����� �������������� ������
#define LEN_SIZE_BYTE 8 //���������� ����, � ������� ������������ ����� ������
#define PACK_SIZE 3 //���������� ��� ��� �������� ������� ��������
#define PACK_SIZE_BYTE 1 //���������� ����, � ������� ������������ ������� ��������
#define MIN_PACK 1
#define MAX_PACK 7