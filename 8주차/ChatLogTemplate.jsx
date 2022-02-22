import React from "react";
import styled from "styled-components";

import ChatLogItem from "./ChatLogItem";


function ChatLogTemplate({chats}) {

  // chats 배열을 받아 저장해 둔 채팅 로그를 보여줌
  // ChatLogItem 컴포넌트 사용
  //const ch=chats.map((chat)=>ChatLogItem(chat));
  // 8주차 실습 구현
  return (
    <ChatContainer>
      
      <h1>Chat Log</h1>
      {chats.map((chat)=>{ return <ChatLogItem chat={chat}/>})}
    </ChatContainer>
  );

}

const ChatContainer = styled.div`  
  width: 500px;
  margin: 10px;
  padding: 20px;

  // 해당 영역 모서리를 둥글게
  border-radius: 15px;
  // 해당 영역 모서리에 그림자
  box-shadow : 0px 5px 15px 10px gray;

  // 해당 영역의 배경색 변경
  background-color:#f3eaaf;
  // 8주차 실습 구현
`;

export default ChatLogTemplate;