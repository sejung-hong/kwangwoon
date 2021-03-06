# Ubuntu installation, Usage of linux commands
  
### man
* 리눅스에 사용되는 명령어와 함수를 설명하는 기능을 하는 명령어.
* 명령어 옆 숫자 : 1~8까지 명령어 함수를 분류하여 나누어서 목적에 맞게 명령어를 사용 
* NAME, SYNOPSIS(실행방법), DESCRIPTION(함수의 기능), OPTIONS, EXAMPLES(함수의 몇가지 예시) 등으로 구성 되어있다.
  

### uname
* 시스템의 다양한 정보를 출력하는 명령어
* -r : 커널의 정보, -m : cpu 아키택처의 정보, -a : 모든 정보
  

### passwd
* 로그인하는 계정의 비밀번호를 변경하는 명령어
* 기존의 passwd 입력 후 바꾸는 passwd 2회 입력한다. 
  

### ls
* 디렉토리 안에 있는 내용을 보여주는 명령어.
* -a : 숨겨져 있는 파일과, 디렉토리 안에 있는 모든 파일을 출력
* -F : 파일의 종류를 표시하여 출력.  (디렉토리는 /, 실행파일은 *)
* -l : 파일의 정보를 자세하고, 다양하게 출력.
  

### pwd
* 현재 진행중인 위치를 표시하는 명령어
  

### mkdir
* 디렉토리를 생성하는 명령어
  

### chmod
* 권한을 파일 혹은 디렉토리에게 부여하는 명령어.
* i) 문자형태로 권한을 주는 방법.
* u : user, g : group, o: other, a : all u 
* '+' : 추가, - : 제거, = : 할당
* r : read, w : write, x : execution
* chmod u-w, g-w,o-r // hello.txt에서 user, grop에서는 write권한이 제거되고, our에서는 read권한이 제거되는 것을 확인할 수 있다.
* ii) 8진수 숫자형태로 권한을 주는 방법.
* 1 : execute 2: write 4: read
* 합으로 권한을 표현한다.
* chmod 664 //our, group에서는 read와 write 권한이, our에서는 read권한이 주어지는 것을 확인할 수 있다. 


### rmdir
* 디렉토리를 제거하는 명령어
  

### cd
* 디렉토리를 이동하는 명령어.
* Cd 다음으로 입력한 명령어로 이동한다. (ex. cd work)
* . : 현재 위치,  .. : 부모 디렉토리의 위치.  ~ : home 디렉토리의 위치. - : 직전에 있던 디렉토리의 위치.

### touch
* 파일이 없으면 빈파일을 생성하고, 파일이 존재하면 파일의 수정시간을 현재시간으로 변경하는 명령어
  
  
### echo
* 문자열을 화면에 출력하는 명령어.
* 파일의 경로를 확인할 수 있다.

### cat
* 파일의 내용을 출력하는 명령어.
* Cat file1.txt file2.txt // 여러 개의 파일 내용을 같이 출력할 수도 있다.
  
  
### more
* 파일의 내용을 출력하는 명령어. (cat과 동일한 역할.)
* cat은 출력했을 때 가장 하단부분, more은 위에서부터 순차적으로 볼 수 있다.
  

### find
* 조건에 만족하는 파일의 이름을 출력해주는 명령어.
* *: 어떠한 문자가 와도 상관없음을 의미한다.
* work디렉토리에 있는 모든 txt 파일을 출력해 주는 것을 볼 수 있다.
  

### grep
* 파일에서 특정한 문자열을 찾고자 할 때 사용하는 명령어.
* 특정한 문자열이 있는 라인만 출력된다.
  

### wc
* 파일의 줄 수, 단어 수, 바이트 수 순차적으로 출력하는 명령어.
* -c : 바이트, -w : 단어 수, -l : 줄 수
  

### cp
* 파일이나 디렉토리를 복사하는 명령어.
* 복사할 대상, 복사할 경로 혹은 파일명을 입력한다. 복사할 대상은 여러 개 명시 가능하다.
* Cp SP_lab/* . : SP_lab에 있는 파일을 현재위치에 복사한다.
  

### mv
* 파일이나 디렉토리를 이동하는 명령어.
* 이동할 대상, 최종적으로 이동할 경로 또는 파일명 입력한다.
* 파일명도 바꿀 수 있다.
* mv ex LINUX // ex라는 파일의 이름이 LINUX로 바뀌는 것을 확인할 수 있다.
  

### rm
* 파일이나 디렉토리를 제거하는 명령어.
* -r : 디렉토리를 삭제하는 명령어. r이 없으면 디렉토리를 삭제 불가, -i : 삭제여부를 확인.
  

### ln
* 링크를 만드는 명령어.
* Cp명령어는 새로운 파일을 만들지만 ln은 같은 파일을 가르키고 있는 대상은 같다.
  
  
### ps
* 현재 실행중인 프로세스 목록 출력하는 명령어.
* -e :  커널 프로세스를 제외한 모든 프로세스 출력
* -f : 모든 format 출력. CMD 필드의 전체 명령어로 출력.
  

### pstree
* 프로세스를 tree형태로 출력하는 명령어
  
  
### exit
* 프로세스를 탈출하는 명령어. Csh모드를 탈출하는 것을 알 수 있다.  
  

### kill
* 프로세스 종료 시그널을 보내는 명령어.
* 강제적으로 프로세스를 종료한다. 
* K -9 4071로 강제 종료되는 것을 볼 수 있다.
  

### time
* 프로그램이 수행되는 시간을 측정하는 명령어.
* Real : 실제 소요시간, user : user 영역에서 수행된 cpu시간, Sys : 커널에서 소비된 cpy시간.
  

### alias
* 기존에 사용되는 명령어를 다른 문자열로 변경하여 사용하는 명령어.
* myls 은 ‘ls -al’ 과 같은 역할을 하는 것을 볼 수 있다. 
  

### vi
* linux 기본 편집기
  
  
### Make
* gcc compiler에서는 컴파일하기 위해 일일이 gcc -o를 입력해주었다면, make 명령어에선 컴파일 과정을 자동화할 수 있다. Makefile에서 컴파일하는 과정을 만들어 주었다. OBJS, CC, EXEC의 변수를 사용하여 타겟을 대체할 수 있다. 
  
  
### gdb
* 프로그램이 수행되는 도중 그 프로그램 내에서 어떤 일이 일어나는지를 볼 수 있게 하는 명령어. (=디버깅을 수행하는 명령어.)  