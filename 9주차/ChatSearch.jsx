// 채팅 키워드 검색창 & 검색 버튼에 대한 컴포넌트 작성
import React, { useState } from "react";
import styled from "styled-components";

import TextField from "@material-ui/core/TextField";


function ChatSearch({onSearchKeySubmit}) {
    const [Keyword,setKeyword] = useState("")


    const onKeywordChange = (e) => {
        setKeyword(e.target.value)
      }

    const onKeySubmit=(e)=>{
        e.preventDefault()
        onSearchKeySubmit(Keyword)
        setKeyword("")
    }
    
    return(
    <div>
    <form onSubmit={(e) => onKeySubmit(e)} >
        <Key>
        <TextField
                name="Keyword"
                onChange={(e) => onKeywordChange(e)}
                value={Keyword}
                id="outlined-multiline-static"
                variant="outlined"
                label="Keyword"
            />

        <Button>Search</Button></Key>
    </form>
    </div>

    );

}

const Key = styled.div`
  margin: 30px;
`;

const Button = styled.button`

  margin-left:70px;
  padding: 18px;
  background-color: white;
  border-color: #c8a951;
  border-radius: 5px;
`;

export default ChatSearch;