# Go-Back-N
Go_Back_N은 sender에서 data를 보낼 때 receiver의 응답을 기다리지 않고 여러 패킷을 전송한다. 확인응답이 안된 가장 오래된 패킷의 순서번호를 base라 하고, 사용되지 않은 가장 작은 순서번호를 nextseqnum이라 한다. 또한, 전송되었지만 아직 확인 응답이 안된 패킷을 위해 크기가 N인 윈도우라 할 수 있다.  
  
![image](https://user-images.githubusercontent.com/66414115/116893510-05484580-ac6c-11eb-92aa-f3ad205c73a5.png)
