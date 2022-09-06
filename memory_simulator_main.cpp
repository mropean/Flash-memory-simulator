#include "memory.h"
const char* data_f = "dataFile.txt";

void 
input_Command() {
	char command[20];	// ��ɾ ����� ����
	int count = 0;		// ��ū ��ġ
	char* tok;			// ��ū ������

	char s_com[10] = "";	// �и��� ��ū�� ���ڿ��� ������ �迭
	int i_com = 0;			// �и��� ��ū�� ���� ������ ����
	char c_com[2] = "";		// �и��� ��ū�� �����͸� ������ �迭

	cout << "�����Ͻ� ��ɾ �Է����ּ��� >> "; cin.getline(command, '\n');

	tok = strtok(command, " ");		// ���ڿ��� ������ �������� ��ū�� �����.
	while (tok) { //��ū�� ���� ������ �ݺ�
		count++;
		switch (count) {
		case 1:  //��ɾ� ��
			strcpy(s_com, tok);		// char�迭 s_com �� ����
			break;
		case 2: //ù ��° �Ű����� 
			i_com = atoi(tok);		// int�� ���� i_com �� ����, ���� ���ڿ��� ����� ���ڴ� �ƽ�Ű�ڵ�� �Ǿ��־�
			break;					// atoi�� ����Ͽ� �ƽ�Ű�ڵ�� ����� ���ڸ� ������ �ٲ��ش�.
		case 3: //�� ��° �Ű�����
			strcpy(c_com, tok);		// char�� ���� c_com �� ����
			break;
		}
		tok = strtok(NULL, " \n"); //���� ��ū���� �Ѿ
	}

	switch (s_com[0]) {
	case 'i': {			// init �Լ� ����
		memory_Init(i_com);
		break;
	}
	case 'w':			// write �Լ� ����
		memory_Write(i_com, c_com);
		break;
	case 'r':			// read �Լ� ����
		memory_Read(i_com);
		break;
	case 'e':			// erase �Լ� ����
		memory_Erase(i_com);
		break;
	case 'o':			// ���α׷� ����
		exit(1);
	default: {
		cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
		cin.clear();	// �Է¹��� ����
		cin.ignore(INT_MAX, '\n');	// �߸��� �Է��� ��� ���۸� ��� ���ѹݺ��� �����Ѵ�.
	}
	}

}

void
memory_Init(int mb_Size) {
	ofstream write_f(data_f);		// c++ �������� ���� ���� �� ����

	for (int var = 0; var < mb_Size; var++) {	// �Էµ� ũ�⸸ŭ �ް�����Ʈ ����
		for (int Block = 0; Block < 64; Block++) {	// 1�ް�����Ʈ�� 64���� ��� ����
			for (int Sector = 0; Sector < 32; Sector++) {	// 1��ϴ� 32���� ���� ����
				for (int i = 0; i < 510; i++) {					// �� �ٴ� 510���� ���� + �ٳѱ����� 512B�� �����.
					write_f << " ";
				}
				if (Block == 63 && Sector == 31) {			// ������ ���������� �ٳѱ��� ���� �߰��� 2B�� �����͸� ����ִ´�.
					write_f << "  ";
					break;
				}
				write_f << endl;
			}
		}
	}
	write_f.close();			// ������� ���� �ݱ�

	cout << mb_Size << " megabytes flash memory" << endl;
	return;
}

void
memory_Write(int sector_num, char *data) {
	char find_data[2] = "";
	FILE* dFile = fopen("dataFile.txt", "r+");	// r+ ���� ��� ������ �̾ �ۼ��� �� �ְ� �Ѵ�.

	fseek(dFile, sector_num * 512, SEEK_SET);	// �Էµ� ������ ��ȣ * 512�� ������ ���ϴ� �ٷ� �̵�
	fgets(find_data, 2, dFile);				// ���� ���Ͽ��� �о�� �����Ͱ�
	if (find_data[0] != ' ') {				// ������ �ƴ϶�� ����� ����
		cout << "overwrite error!!!" << endl;
		fclose(dFile);							// ������ �ݴ´�.
		return;
	}
	fseek(dFile, -1, SEEK_CUR);				// �տ��� �������� ����Ʈ�� �̵������� �ٽ� �ڷ� ���� �����͸� �ѱ��.
	fputs(data, dFile);						// ���� �����Ϳ��� �Էµ� ������ �Է�
	fclose(dFile);							// ������ �ݴ´�.

	cout << "done" << endl;
	return;
}

void
memory_Read(int sector_num) {
	char data[2] = "";
	FILE* dFile = fopen("dataFile.txt", "r");	// r ���� ������ ��� ���� �б⸸ �����ϰ� �Ѵ�.
	fseek(dFile, sector_num * 512, SEEK_SET);	// �Էµ� ������ ��ȣ * 512�� ������ ���ϴ� �ٷ� �̵�
	fgets(data, 2, dFile);						// ������ �����͸� �о���µ�
	if (data[0] == ' ') {						// �����̶�� �ҷ��� �����Ͱ� ������ �Լ��� �����Ѵ�.
		cout << "�ҷ��� �����Ͱ� �����ϴ�!" << endl;
		fclose(dFile);								// ������ �ݴ´�.
		return;
	}
	cout << data[0] << endl;					// ȭ��� �ҷ��� ������ ���
	fclose(dFile);								// ������ �ݴ´�.
}

void
memory_Erase(int block_num) {
	int b_start, b_end;							// ��Ͼ��� ���� ���Ϳ� �� ������ ���� ��
	FILE* dFile = fopen("dataFile.txt", "r+");
	
	// ����� ������ ��ϴ���
	b_start = block_num * 32;	// ����� ù��° ������ ��ġ
	b_end = b_start + 31;		// ����� ������ ������ ��ġ 0~31

	fseek(dFile, b_start * 512, SEEK_SET);	// ������� ����� ù��° ���ͷ� �̵�
	// ù��° ������ �Ǿտ� �����Ͱ� �ִ�
	for (int Sector = 0; Sector < 32; Sector++) {	// 1��ϴ� 32���� ���� ����
		for (int i = 0; i < 510; i++) {					// �� �ٴ� 510���� ���� + �ٳѱ����� 512B�� �����.
			fputs(" ", dFile);
		}
		fputs("\n", dFile);
	}
	fclose(dFile);			// ������ �ݴ´�.
	cout << block_num << "-th block erase." << endl;
	return;
}