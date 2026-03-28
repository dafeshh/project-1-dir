//#define UNICODE
//#define _UNICODE
#include <windows.h> // thu vien nay co chua winapi 
#include <iostream> // thu vien dung de nhap xuat c++ su dung std::
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <set>
#include <string.h>

void print_LastModifyTime(WIN32_FIND_DATAA data){
	SYSTEMTIME SysTime;
	FileTimeToSystemTime(&data.ftLastWriteTime, &SysTime);
	printf("%02d/%02d/%d  %02d:%02d    ",
    SysTime.wMonth, SysTime.wDay, SysTime.wYear,
    SysTime.wHour +7, SysTime.wMinute);
}

void print_FileName(WIN32_FIND_DATAA data){
	std::cout << data.cFileName << std::endl;
}

void print_FileAtribute(WIN32_FIND_DATAA data){
	if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		std::cout << "<DIR>";
	else if(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
		std::cout << "<JUNCTION>";
	else
		std::cout << "     ";
}

void print_FileSize(WIN32_FIND_DATAA data, LARGE_INTEGER filesize){
	if(!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
		filesize.u.LowPart = data.nFileSizeLow;
		filesize.u.HighPart = data.nFileSizeHigh;
		printf("%9ld ", filesize.QuadPart);
	}
	else 
	std::cout <<  "          "; 
}

void File_and_Folder_count_and_total_memories(WIN32_FIND_DATAA data, LARGE_INTEGER filesize, char searchpath[], int &DIR, int &FILE, long long &CURFILEMEM){
	DIR = 0, FILE = 0, CURFILEMEM = 0;
	HANDLE hFind  = FindFirstFile(searchpath, &data); 
	if(hFind != INVALID_HANDLE_VALUE){
		do{
			if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				DIR++;
			else{
				FILE++;
				filesize.u.LowPart = data.nFileSizeLow;
				filesize.u.HighPart = data.nFileSizeHigh;
				CURFILEMEM += filesize.QuadPart;
			}
		}while(FindNextFile(hFind, &data));
	}
	FindClose(hFind);
}

// void print_JunctionLink(WIN32_FIND_DATAA data){
// 	HANDLE hFile = 			
// 	if(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){

// 	}
// }

void dir(WIN32_FIND_DATAA data, LARGE_INTEGER filesize, TCHAR path[], TCHAR Disk[], int checkB, int checkS){
	int DIR_Cnt, FILE_Cnt;
	long long Total_FileMem;
	ULARGE_INTEGER FreeDiskMem;
	char searchpath[255];
	strcpy(searchpath, path);
	strcat(searchpath, "\\*");
	TCHAR CurPath[MAX_PATH];
	DWORD GetCurPath = GetCurrentDirectory(MAX_PATH, CurPath);
	HANDLE hFind  = FindFirstFile(searchpath, &data); 
	// HANDLE la 1 ID hoac la con tro ma windowns cap cho minh de thao tac voi 1 tai nguyen nao do.	
	/*
	cu phap cua FindFirstFile:
	
	HANDLE FindFirstFileA(
		[in]  LPCSTR             lpFileName,
		[out] LPWIN32_FIND_DATAA lpFindFileData
	);
	
	Tham so nhan vao la lpFileName se la 1 duong dan hoac ten file, tham so nay khong nen la NULL hoac la chuoi khong xac dinh.
	
	Tham so tra ve la lpFindFileData se la 1 con tro den WIN32_FIND_DATA chua thong tin ve file hoac thu muc tim thay.
	
	Neu tim  thanh cong thi HANLDE se tra ve 1 ID cho lan goi tiep theo den FindNextFile hoac FindClose.
	
	Neu that bai thi se tra ve INVALID_HANDLE_VALUE
	*/
	DWORD VolumeSerialNumber=0;
	GetVolumeInformation(Disk, NULL, NULL, &VolumeSerialNumber, NULL, NULL, NULL, NULL);
	if(!checkB){
		printf(" Volume in drive %c is Windows-SSD\n", Disk[0]); 
		printf(" Volume Serial Number is %04X-%04X\n\n", HIWORD(VolumeSerialNumber), LOWORD(VolumeSerialNumber));
	}
	
	
	if(hFind != INVALID_HANDLE_VALUE){
		if(!checkB)
			printf(" Directory of %s\n\n", path);
		do{
			if(checkB){
				if(checkS)
					std::cout<< path << "\\";
				print_FileName(data);
			}
			else{
				print_LastModifyTime(data);
				print_FileAtribute(data);
				print_FileSize(data, filesize);
				print_FileName(data);
			}
			
		}while(FindNextFile(hFind, &data));
		/*
		cau truc FindNextFile:
		
		BOOL FindNextFileA(
			[in]  HANDLE             hFindFile,
			[out] LPWIN32_FIND_DATAA lpFindFileData
		);
		
		Tham so vao hFindFile la ID co duoc tu HANDLE FindFirstFile.
		
		Tham so tra ve lpFindFileData la con tro den WIN32_FIND_DATA chua thong tin ve file hoac thu muc con duoc tim thay.
		*/
		FindClose(hFind);
		if(checkB)
			return;
		// FindClose se tra HANDLE lai cho windows de tranh bi loi giong nhu ngung cap phat con tro de tranh lam day bo nho.
		File_and_Folder_count_and_total_memories(data, filesize, searchpath, DIR_Cnt, FILE_Cnt, Total_FileMem);
		printf("%16ld File(s) %14ld bytes\n", FILE_Cnt, Total_FileMem);
		GetDiskFreeSpaceExA("D:\\", &FreeDiskMem, NULL, NULL);
		printf("%16ld  Dir(s) %14llu bytes free\n\n", DIR_Cnt, FreeDiskMem.QuadPart);
	}
	else{
		printf(" Directory of %s\n\n", CurPath);
		std::cout << "File Not Found\n" << std::endl;
		FindClose(hFind);
	}
} 

void Parameter_and_Switch_format_Check(int argc, char *argv[], int &Dsl, int Asl[]){
	
	for(int i = 1; i < argc; i++){
		TCHAR *arg = argv[i];

		Dsl = 0;

		if(arg[0] != '/')
			continue;
		else{	
//			std::cout << "arg = " << arg << std::endl;
			for(int i = 0; i < strlen(arg); i++){
				if(arg[i] == '/'){
					Asl[Dsl] = i; 
//					std::cout << Asl[Dsl] << " " << Dsl << std::endl;
					Dsl++;
				}
			}
			Asl[Dsl] = strlen(arg) ;
			// std::cout << "Dsl = " <<Dsl << std::endl;
//			std::cout << strlen(arg) << " " << std::endl;
			for(int i = 0; i < Dsl; i++){
//				std::cout << Asl[i] << " " << std::endl;
		//		std::cout << arg[Asl[i]+1] << " " << std::endl;
				if(arg[Asl[i]+1] != 'A' && arg[Asl[i]+1] != 'B' && arg[Asl[i]+1] != 'C' && arg[Asl[i]+1] != 'D' && arg[Asl[i]+1] != 'L' && arg[Asl[i]+1] != 'N' && arg[Asl[i]+1] != 'O' && arg[Asl[i]+1] != 'P' && arg[Asl[i]+1] != 'Q' && arg[Asl[i]+1] != 'R' && arg[Asl[i]+1] != 'D' && arg[Asl[i]+1] != 'T' && arg[Asl[i]+1] != 'W' && arg[Asl[i]+1] != 'X' && arg[Asl[i]+1] != '?' 
				&& arg[Asl[i]+1] != 'a' && arg[Asl[i]+1] != 'b' && arg[Asl[i]+1] != 'c' && arg[Asl[i]+1] != 'd' && arg[Asl[i]+1] != 'l' && arg[Asl[i]+1] != 'n' && arg[Asl[i]+1] != 'o' && arg[Asl[i]+1] != 'p' && arg[Asl[i]+1] != 'q' && arg[Asl[i]+1] != 'r' && arg[Asl[i]+1] != 's' && arg[Asl[i]+1] != 't' && arg[Asl[i]+1] != 'w' && arg[Asl[i]+1] != 'x')
				{
					std::cout << "Invalid switch - \"" << arg[Asl[i]+1] << "\"\n\n";
					std::exit(EXIT_FAILURE);
				}
			}

			for(int i = 0; i < Dsl; i++){
				if(Asl[i+1] - Asl[i] <= 2){
				// std::cout << "arg[Asl[i]+1] = " << Asl[i+1] - Asl[i] << std::endl;
					continue;
				}
				if(arg[Asl[i]+1] == 'A' || arg[Asl[i]+1] == 'a'){
					for(int j = Asl[i]+2; j < Asl[i+1]; j++){
						if(arg[j] != 'D' && arg[j] != 'H' && arg[j] != 'S' && arg[j] != 'L' && arg[j] != '-'   
						&& arg[j] != 'd' && arg[j] != 'h' && arg[j] != 's' && arg[j] != 'l' && arg[j] != ':'
						&& arg[j] != 'R' && arg[j] != 'A' && arg[j] != 'I' && arg[j] != 'O' 
						&& arg[j] != 'r' && arg[j] != 'a' && arg[j] != 'i' && arg[j] != 'o' ){
							std::cout << "Parameter format not correct - \"" << arg[j] << "\"\n\n";
							std::exit(EXIT_FAILURE);
						}	
					}
				}
				
				
			}
		}
	}
}


void printHelp(){
	std::cout <<"Displays a list of files and subdirectories in a directory.\n\n"
				"DIR [drive:][path][filename] [/A[[:]attributes]] [/B] [/C] [/D] [/L] [/N]\n"
				"  [/O[[:]sortorder]] [/P] [/Q] [/R] [/S] [/T[[:]timefield]] [/W] [/X] [/4]\n\n" 
				"  [drive:][path][filename]\n"
				"              Specifies drive, directory, and/or files to list.\n\n"
				"  /A          Displays files with specified attributes.\n"
				"  attributes   D  Directories                R  Read-only files\n"
				"               H  Hidden files               A  Files ready for archiving\n"
				"               S  System files               I  Not content indexed files\n"
				"               L  Reparse Points             O  Offline files\n"
				"  /B          Uses bare format (no heading information or summary).\n"
				"  /C          Display the thousand separator in file sizes.  This is the\n"
				"              default.  Use /-C to disable display of separator.\n"
				"  /D          Same as wide but files are list sorted by column.\n"
				"  /L          Uses lowercase.\n"
				"  /N          New long list format where filenames are on the far right.\n"
				"  /O          List by files in sorted order.\n"
				"  sortorder    N  By name (alphabetic)       S  By size (smallest first)\n"
				"               E  By extension (alphabetic)  D  By date/time (oldest first)\n"
				"               G  Group directories first    -  Prefix to reverse order\n"
				"  /P          Pauses after each screenful of information.\n"
				"  /Q          Display the owner of the file.\n"
				"  /R          Display alternate data streams of the file.\n"
				"  /S          Displays files in specified directory and all subdirectories.\n"
				"  /T          Controls which time field displayed or used for sorting\n"
				"  timefield   C  Creation\n"
				"              A  Last Access\n"
				"              W  Last Written\n"
				"  /W          Uses wide list format.\n"
				"  /X          This displays the short names generated for non-8dot3 file\n"
				"              names.  The format is that of /N with the short name inserted\n"
				"              before the long name. If no short name is present, blanks are\n"
				"              displayed in its place.\n\n"
				"Switches may be preset in the DIRCMD environment variable.  Override\n"
				"preset switches by prefixing any switch with - (hyphen)--for example, /-W.\n\n";
				std::exit(EXIT_SUCCESS);
}

//
void Displays_files_with_specified_attributes(WIN32_FIND_DATAA data, LARGE_INTEGER filesize, TCHAR path[], TCHAR Disk[], int argc, char *argv[], int &Dsl, int Asl[], int checkB, int checkS){
	int DIR_Cnt, FILE_Cnt;
	long long Total_FileMem;
	ULARGE_INTEGER FreeDiskMem;
	char searchpath[255];
	strcpy(searchpath, path);
	strcat(searchpath, "\\*");
	TCHAR CurPath[MAX_PATH];
	DWORD GetCurPath = GetCurrentDirectory(MAX_PATH, CurPath);
	HANDLE hFind  = FindFirstFile(searchpath, &data); 
	

	
	for(int i = 1; i < argc; i++){
		TCHAR *arg = argv[i];
		
		Dsl = 0;

		for(int t = 0; t < strlen(arg); t++){
				if(arg[t] == '/'){
					Asl[Dsl] = t; 
//					std::cout << Asl[Dsl] << " " << Dsl << std::endl;
					Dsl++;
				}
			}
			Asl[Dsl] = strlen(arg) ;


		for(int k = 0; k < Dsl; k++){
			if(argc < 2 || ((arg[Asl[k]+1] == 'A' || arg[Asl[k]+1] == 'a') &&  Asl[k+1] - Asl[k] == 2)){
				dir(data, filesize, path, Disk, checkB, checkS);	
				// std::cout << argv[i] << " " << argc << " note " << Asl[k+1] - Asl[k] << " " << Dsl << " " <<  Asl[k] << " " << Asl[k+1]<< " " << arg[2];
				// std::cout << argv[i] << " " << argc << " " << Asl[i+1] - Asl[i];
				break;
			}
			
			for(int j = Asl[k]+2; j <= Asl[k+1]; j++){
				// std::cout << argv[i] << " " << argc << " note " << Asl[k+1] - Asl[k] << " " << Dsl << " " <<  Asl[k] << " " << Asl[k+1]<< " " << arg[2];
				switch(arg[j]){
					
					case 'D': case 'd':
						if(hFind != INVALID_HANDLE_VALUE){
							// std::cout << argv[i] << " " << argc << " note " << Asl[i+1] - Asl[i] << " " << Dsl << " " <<  Asl[i] << " " << Asl[i+1];
							do{
								// std::cout << "cac " << checkB << std::endl;
								if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
									// std::cout << checkB << std::endl;
									if(checkB){
										if(checkS)
											std::cout << path << "\\";
										print_FileName(data);
 									}
									else{
										print_LastModifyTime(data);
										print_FileAtribute(data);
										print_FileSize(data, filesize);
										print_FileName(data);
									}
								}
							}while(FindNextFile(hFind, &data));	
						}
						FindClose(hFind);
						if(checkB)
							break;
						File_and_Folder_count_and_total_memories(data, filesize, searchpath, DIR_Cnt, FILE_Cnt, Total_FileMem);
						printf("%16ld File(s) %14ld bytes\n", FILE_Cnt, Total_FileMem);
						GetDiskFreeSpaceExA("D:\\", &FreeDiskMem, NULL, NULL);
						printf("%16ld  Dir(s) %14llu bytes free\n\n", DIR_Cnt, FreeDiskMem.QuadPart);
						break;
					
					case 'H': case 'h':
						if(hFind != INVALID_HANDLE_VALUE){
							do{
								if(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN){
									if(checkB){
										print_FileName(data);
 									}
									else{
										print_LastModifyTime(data);
										print_FileAtribute(data);
										print_FileSize(data, filesize);
										print_FileName(data);
									}
								}
							}while(FindNextFile(hFind, &data));
						}
						FindClose(hFind);
						if(checkB)
							break;
						File_and_Folder_count_and_total_memories(data, filesize, searchpath, DIR_Cnt, FILE_Cnt, Total_FileMem);
						printf("%16ld File(s) %14ld bytes\n", FILE_Cnt, Total_FileMem);
						GetDiskFreeSpaceExA("D:\\", &FreeDiskMem, NULL, NULL);
						printf("%16ld  Dir(s) %14llu bytes free\n\n", DIR_Cnt, FreeDiskMem.QuadPart);
						break;
					
					case 'S': case 's':
						if(hFind != INVALID_HANDLE_VALUE){
							do{
								if(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM){
									if(checkB){
										print_FileName(data);
 									}
									else{
										print_LastModifyTime(data);
										print_FileAtribute(data);
										print_FileSize(data, filesize);
										print_FileName(data);
									}
								}
							}while(FindNextFile(hFind, &data));	
						}

						FindClose(hFind);
						if(checkB)
							break;

						File_and_Folder_count_and_total_memories(data, filesize, searchpath, DIR_Cnt, FILE_Cnt, Total_FileMem);
						printf("%16ld File(s) %14ld bytes\n", FILE_Cnt, Total_FileMem);
						GetDiskFreeSpaceExA("D:\\", &FreeDiskMem, NULL, NULL);
						printf("%16ld  Dir(s) %14llu bytes free\n\n", DIR_Cnt, FreeDiskMem.QuadPart);
						break;

					case 'L': case 'l':
						if(hFind != INVALID_HANDLE_VALUE){
							do{
								if(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
									if(checkB){
										print_FileName(data);
 									}
									else{
										print_LastModifyTime(data);
										print_FileAtribute(data);
										print_FileSize(data, filesize);
										print_FileName(data);
									}
								}
							}while(FindNextFile(hFind, &data));	
						}
						FindClose(hFind);

						if(checkB)
							break;

						File_and_Folder_count_and_total_memories(data, filesize, searchpath, DIR_Cnt, FILE_Cnt, Total_FileMem);
						printf("%16ld File(s) %14ld bytes\n", FILE_Cnt, Total_FileMem);
						GetDiskFreeSpaceExA("D:\\", &FreeDiskMem, NULL, NULL);
						printf("%16ld  Dir(s) %14llu bytes free\n\n", DIR_Cnt, FreeDiskMem.QuadPart);
						break;

					case 'R': case 'r':
						if(hFind != INVALID_HANDLE_VALUE){
							do{
								if(data.dwFileAttributes & FILE_ATTRIBUTE_READONLY){
									if(checkB){
										print_FileName(data);
									}
									else{
										print_LastModifyTime(data);
										print_FileAtribute(data);
										print_FileSize(data, filesize);
										print_FileName(data);
									}
								}
							}while(FindNextFile(hFind, &data));	
						}
						FindClose(hFind);

						if(checkB)
							break;

						File_and_Folder_count_and_total_memories(data, filesize, searchpath, DIR_Cnt, FILE_Cnt, Total_FileMem);
						printf("%16ld File(s) %14ld bytes\n", FILE_Cnt, Total_FileMem);
						GetDiskFreeSpaceExA("D:\\", &FreeDiskMem, NULL, NULL);
						printf("%16ld  Dir(s) %14llu bytes free\n\n", DIR_Cnt, FreeDiskMem.QuadPart);
						break;

					case 'A': case 'a':
						if(hFind != INVALID_HANDLE_VALUE){
							do{
								if(data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE){
									if(checkB){
										print_FileName(data);
									}
									else{
										print_LastModifyTime(data);
										print_FileAtribute(data);
										print_FileSize(data, filesize);
										print_FileName(data);
									}
								}
							}while(FindNextFile(hFind, &data));	
						}
						FindClose(hFind);
						if(checkB)
							break;
						File_and_Folder_count_and_total_memories(data, filesize, searchpath, DIR_Cnt, FILE_Cnt, Total_FileMem);
						printf("%16ld File(s) %14ld bytes\n", FILE_Cnt, Total_FileMem);
						GetDiskFreeSpaceExA("D:\\", &FreeDiskMem, NULL, NULL);
						printf("%16ld  Dir(s) %14llu bytes free\n\n", DIR_Cnt, FreeDiskMem.QuadPart);
						break;

					case 'I': case 'i':
						if(hFind != INVALID_HANDLE_VALUE){
							do{
								if(data.dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED){
									if(checkB){
										print_FileName(data);
 									}
									else{
										print_LastModifyTime(data);
										print_FileAtribute(data);
										print_FileSize(data, filesize);
										print_FileName(data);
									}
								}
							}while(FindNextFile(hFind, &data));	
						}
						FindClose(hFind);
						if(checkB)
							break;
						File_and_Folder_count_and_total_memories(data, filesize, searchpath, DIR_Cnt, FILE_Cnt, Total_FileMem);
						printf("%16ld File(s) %14ld bytes\n", FILE_Cnt, Total_FileMem);
						GetDiskFreeSpaceExA("D:\\", &FreeDiskMem, NULL, NULL);
						printf("%16ld  Dir(s) %14llu bytes free\n\n", DIR_Cnt, FreeDiskMem.QuadPart);
						break;

					case 'O': case 'o':
						if(hFind != INVALID_HANDLE_VALUE){
							do{
								if(data.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE){
									if(checkB){
										print_FileName(data);
 									}
									else{
										print_LastModifyTime(data);
										print_FileAtribute(data);
										print_FileSize(data, filesize);
										print_FileName(data);
									}
								}
							}while(FindNextFile(hFind, &data));	
						}
						FindClose(hFind);
						if(checkB)
							break;
						File_and_Folder_count_and_total_memories(data, filesize, searchpath, DIR_Cnt, FILE_Cnt, Total_FileMem);
						printf("%16ld File(s) %14ld bytes\n", FILE_Cnt, Total_FileMem);
						GetDiskFreeSpaceExA("D:\\", &FreeDiskMem, NULL, NULL);
						printf("%16ld  Dir(s) %14llu bytes free\n\n", DIR_Cnt, FreeDiskMem.QuadPart);
						break;
				}	
			}
		}
	}
}

void print_bare_format(WIN32_FIND_DATAA data ,TCHAR path[], int CheckA, int CheckS){
	char searchpath[255];
	strcpy(searchpath, path);
	strcat(searchpath, "\\*");
	HANDLE hFind  = FindFirstFile(searchpath, &data); 
	if(CheckA || CheckS)
		return;
	if(hFind != INVALID_HANDLE_VALUE){
		do{
			print_FileName(data);
		}while(FindNextFile(hFind, &data));
	}
	FindClose(hFind);
}

void Displays_files_in_specified_directory_and_all_subdirectories(WIN32_FIND_DATAA data, LARGE_INTEGER filesize, TCHAR path[], TCHAR Disk[], int argc, char *argv[], int &Dsl, int Asl[], int &d, int checkA, int checkB){
	char searchpath[255];
	strcpy(searchpath, path);
	strcat(searchpath, "\\*");
	TCHAR CurPath[MAX_PATH];
	DWORD GetCurPath = GetCurrentDirectory(MAX_PATH, CurPath);
	TCHAR folder_path[255];
	TCHAR search_folder_path[255];
	HANDLE hFind  = FindFirstFile(searchpath, &data); 
	if(hFind != INVALID_HANDLE_VALUE){
		do{
			if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if(!strcmp(data.cFileName, "..") || !strcmp(data.cFileName, "."))
					continue;
				// if(!strcmp(data.cFileName, ".")){
				// 	if(d == 0){
				// 	strcpy(folder_path, CurPath);
				// 	dir(data, filesize, folder_path, Disk, checkB, 1);
				// 	d++;
				// 	}
				// 	else continue;
				// }
				else{
					strcpy(folder_path, path);
					strcat(folder_path, "\\");
					strcat(folder_path, data.cFileName);

					Displays_files_in_specified_directory_and_all_subdirectories(data, filesize, folder_path, Disk, argc, argv, Dsl, Asl, d, checkA, checkB);

					if(checkA)
						
						Displays_files_with_specified_attributes(data, filesize, folder_path, Disk, argc, argv, Dsl, Asl, checkB, 1);
					else
						dir(data, filesize, folder_path, Disk, checkB, 1);
		
					
				}
				
			}
		}while(FindNextFile(hFind, &data));	
	}
	//if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	FindClose(hFind);
}

int main(int argc, char *argv[]){
	WIN32_FIND_DATAA data;	
	/*WIN32_FIND_DATA la 1 struct cua thu vien windows.h su dung de dua ra thong tin cua file nhu ten file, kich thuoc, thoi gian chinh sua.
	struct: 
	typedef struct _WIN32_FIND_DATAA {
		DWORD    dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD    nFileSizeHigh;
		DWORD    nFileSizeLow;
		DWORD    dwReserved0;
		DWORD    dwReserved1;
		CHAR     cFileName[MAX_PATH];
		CHAR     cAlternateFileName[14];
		DWORD    dwFileType; // Obsolete. Do not use.
		DWORD    dwCreatorType; // Obsolete. Do not use
		WORD     wFinderFlags; // Obsolete. Do not use
	} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
	*/
	int Dsl = 0, Asl[100], d = 0, checkA = 0, checkB = 0, checkS = 0;
	LARGE_INTEGER filesize;
	TCHAR path[MAX_PATH], Disk[4], inpath[MAX_PATH];
	DWORD GetPath = GetCurrentDirectory(MAX_PATH, path);
	memcpy(Disk, path, 3);
	
	Disk[3] = '\0';

	
	if(argc <= 1){
		dir(data, filesize, path, Disk, checkB, checkS);
	}

		Parameter_and_Switch_format_Check(argc, argv, Dsl, Asl);
	
	for(int i = 1; i < argc; i++){
		if (std::string(argv[i]).find("/b") != std::string::npos || std::string(argv[i]).find("/B") != std::string::npos)
			checkB = 1;

		if (std::string(argv[i]).find("/a") != std::string::npos || std::string(argv[i]).find("/A") != std::string::npos)
			checkA = 1;

		if (std::string(argv[i]).find("/s") != std::string::npos || std::string(argv[i]).find("/S") != std::string::npos)
			checkS = 1;
	}

	for(int i = 1; i < argc; i++){
		TCHAR *arg = argv[i];
		
			

		if (arg[0] == '/'){
		// std::cout << argv[i] << " " << arg[0] << " djtmecuocdoi " << arg << std::endl;	
			if (!strcmp(arg, "/?"))
				printHelp(); 

			else if(checkS){
				Displays_files_in_specified_directory_and_all_subdirectories(data, filesize, path, Disk, argc, argv, Dsl, Asl, d, checkA, checkB);
			}

			else if(arg[1] == 'A' || arg[1] == 'a'){
// std::cout << argv[i] << " " << arg[0] << " djtmecuocdoi " << arg << std::endl;	
				Displays_files_with_specified_attributes(data, filesize, path, Disk, argc, argv, Dsl, Asl, checkB, checkS);
			}

//				Parameter_and_Switch_format_Check(argc, argv);
			else if(arg[1] == 'B' || arg[1] == 'b')
				print_bare_format(data ,path, checkA, checkS);
			
			

			// else if(arg[1] == 'S' || arg[1] == 's')
			// 	Displays_files_in_specified_directory_and_all_subdirectories(data, filesize, path, Disk, argc, argv, Dsl, Asl, d, checkA, checkB);
		}
		else{
			if(!strcmp(arg, "\\"))
				strcpy(path,Disk);
			else{
				strcpy(path,arg);
			}
			if(argc <= 2)
				dir(data, filesize, path, Disk, checkB, checkS);	
			
		}
	}

	return 0;
}
