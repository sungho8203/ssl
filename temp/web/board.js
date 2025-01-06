// 게시글 데이터 (실제로는 서버에서 관리)
let posts = [];
let currentId = 1;

// 게시글 목록 표시
function displayPosts() {
    const postsContainer = document.getElementById('posts');
    postsContainer.innerHTML = '';

    posts.forEach(post => {
        const postElement = document.createElement('div');
        postElement.className = 'board-item';
        postElement.innerHTML = `
            <span class="col-id">${post.id}</span>
            <span class="col-title">${post.title}</span>
            <span class="col-author">${post.author}</span>
            <span class="col-date">${post.date}</span>
        `;
        
        // 게시글 클릭 이벤트
        postElement.addEventListener('click', () => {
            alert(`제목: ${post.title}\n내용: ${post.content}`);
        });
        
        postsContainer.appendChild(postElement);
    });
}

// 글쓰기 모달 표시
document.getElementById('writeBtn').addEventListener('click', () => {
    document.getElementById('writeModal').style.display = 'block';
});

// 글쓰기 취소
document.getElementById('cancelPost').addEventListener('click', () => {
    document.getElementById('writeModal').style.display = 'none';
});

// 글쓰기 제출
document.getElementById('submitPost').addEventListener('click', () => {
    const title = document.getElementById('postTitle').value;
    const author = document.getElementById('postAuthor').value;
    const content = document.getElementById('postContent').value;
    
    if (!title || !author || !content) {
        alert('모든 필드를 입력해주세요.');
        return;
    }
    
    // 새 게시글 추가
    const newPost = {
        id: currentId++,
        title,
        author,
        content,
        date: new Date().toLocaleDateString()
    };
    
    posts.unshift(newPost);  // 배열 앞에 추가
    displayPosts();
    
    // 모달 닫기 및 폼 초기화
    document.getElementById('writeModal').style.display = 'none';
    document.getElementById('postTitle').value = '';
    document.getElementById('postAuthor').value = '';
    document.getElementById('postContent').value = '';
});

// 초기 게시글 목록 표시
displayPosts(); 