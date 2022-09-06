#include "memory.h"
const char* data_f = "dataFile.txt";

void 
input_Command() {
	char command[20];	// 명령어가 저장될 변수
	int count = 0;		// 토큰 위치
	char* tok;			// 토큰 포인터

	char s_com[10] = "";	// 분리한 토큰의 문자열을 저장할 배열
	int i_com = 0;			// 분리한 토큰의 수를 저장할 변수
	char c_com[2] = "";		// 분리한 토큰의 데이터를 저장할 배열

	cout << "실행하실 명령어를 입력해주세요 >> "; cin.getline(command, '\n');

	tok = strtok(command, " ");		// 문자열을 공백을 기준으로 토큰을 만든다.
	while (tok) { //토큰이 없을 때까지 반복
		count++;
		switch (count) {
		case 1:  //명령어 부
			strcpy(s_com, tok);		// char배열 s_com 에 저장
			break;
		case 2: //첫 번째 매개변수 
			i_com = atoi(tok);		// int형 변수 i_com 에 저장, 보통 문자열에 저장된 숫자는 아스키코드로 되어있어
			break;					// atoi를 사용하여 아스키코드로 저장된 숫자를 정수로 바꿔준다.
		case 3: //두 번째 매개변수
			strcpy(c_com, tok);		// char형 변수 c_com 에 저장
			break;
		}
		tok = strtok(NULL, " \n"); //다음 토큰으로 넘어감
	}

	switch (s_com[0]) {
	case 'i': {			// init 함수 실행
		memory_Init(i_com);
		break;
	}
	case 'w':			// write 함수 실행
		memory_Write(i_com, c_com);
		break;
	case 'r':			// read 함수 실행
		memory_Read(i_com);
		break;
	case 'e':			// erase 함수 실행
		memory_Erase(i_com);
		break;
	case 'o':			// 프로그램 종료
		exit(1);
	default: {
		cout << "잘못 입력하셨습니다." << endl;
		cin.clear();	// 입력버퍼 비우기
		cin.ignore(INT_MAX, '\n');	// 잘못된 입력일 경우 버퍼를 비워 무한반복을 예방한다.
	}
	}

}

void
memory_Init(int mb_Size) {
	ofstream write_f(data_f);		// c++ 형식으로 파일 생성 및 열기

	for (int var = 0; var < mb_Size; var++) {	// 입력된 크기만큼 메가바이트 생성
		for (int Block = 0; Block < 64; Block++) {	// 1메가바이트당 64개의 블록 생성
			for (int Sector = 0; Sector < 32; Sector++) {	// 1블록당 32개의 섹터 생성
				for (int i = 0; i < 510; i++) {					// 한 줄당 510개의 공백 + 줄넘김으로 512B를 만든다.
					write_f << " ";
				}
				if (Block == 63 && Sector == 31) {			// 파일의 마지막줄은 줄넘김이 없게 추가로 2B의 데이터를 집어넣는다.
					write_f << "  ";
					break;
				}
				write_f << endl;
			}
		}
	}
	write_f.close();			// 열어놓은 파일 닫기

	cout << mb_Size << " megabytes flash memory" << endl;
	return;
}

void
memory_Write(int sector_num, char *data) {
	char find_data[2] = "";
	FILE* dFile = fopen("dataFile.txt", "r+");	// r+ 모드로 열어서 파일을 이어서 작성할 수 있게 한다.

	fseek(dFile, sector_num * 512, SEEK_SET);	// 입력된 섹터의 번호 * 512로 파일의 원하는 줄로 이동
	fgets(find_data, 2, dFile);				// 만약 파일에서 읽어온 데이터가
	if (find_data[0] != ' ') {				// 공백이 아니라면 덮어쓰기 에러
		cout << "overwrite error!!!" << endl;
		fclose(dFile);							// 파일을 닫는다.
		return;
	}
	fseek(dFile, -1, SEEK_CUR);				// 앞에서 읽으려고 바이트를 이동했으니 다시 뒤로 파일 포인터를 넘긴다.
	fputs(data, dFile);						// 현재 포인터에서 입력된 데이터 입력
	fclose(dFile);							// 파일을 닫는다.

	cout << "done" << endl;
	return;
}

void
memory_Read(int sector_num) {
	char data[2] = "";
	FILE* dFile = fopen("dataFile.txt", "r");	// r 모드로 파일을 열어서 오직 읽기만 가능하게 한다.
	fseek(dFile, sector_num * 512, SEEK_SET);	// 입력된 섹터의 번호 * 512로 파일의 원하는 줄로 이동
	fgets(data, 2, dFile);						// 파일의 데이터를 읽어오는데
	if (data[0] == ' ') {						// 공백이라면 불러올 데이터가 없으니 함수를 종료한다.
		cout << "불러올 데이터가 없습니다!" << endl;
		fclose(dFile);								// 파일을 닫는다.
		return;
	}
	cout << data[0] << endl;					// 화면상에 불러온 데이터 출력
	fclose(dFile);								// 파일을 닫는다.
}

void
memory_Erase(int block_num) {
	int b_start, b_end;							// 블록안의 시작 섹터와 끝 섹터의 변수 값
	FILE* dFile = fopen("dataFile.txt", "r+");
	
	// 지우기 연산은 블록단위
	b_start = block_num * 32;	// 블록의 첫번째 섹터의 위치
	b_end = b_start + 31;		// 블록의 마지막 섹터의 위치 0~31

	fseek(dFile, b_start * 512, SEEK_SET);	// 지우려는 블록의 첫번째 섹터로 이동
	// 첫번째 섹터의 맨앞에 포인터가 있다
	for (int Sector = 0; Sector < 32; Sector++) {	// 1블록당 32개의 섹터 생성
		for (int i = 0; i < 510; i++) {					// 한 줄당 510개의 공백 + 줄넘김으로 512B를 만든다.
			fputs(" ", dFile);
		}
		fputs("\n", dFile);
	}
	fclose(dFile);			// 파일을 닫는다.
	cout << block_num << "-th block erase." << endl;
	return;
}