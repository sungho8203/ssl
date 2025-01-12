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
            ${post.image ? `<img src="${post.image}" class="post-image" alt="게시글 이미지">` : ''}
            <span class="col-title">${post.title}</span>
            <span class="col-author">${post.author}</span>
            <span class="col-date">${post.date}</span>
        `;
        
        // 게시글 클릭 이벤트
        postElement.addEventListener('click', () => {
            showPostDetail(post);
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

// 이미지 미리보기
document.getElementById('postImage').addEventListener('change', function(e) {
    const file = e.target.files[0];
    if (file) {
        const reader = new FileReader();
        reader.onload = function(e) {
            const preview = document.getElementById('imagePreview');
            preview.innerHTML = `<img src="${e.target.result}" alt="Preview">`;
        }
        reader.readAsDataURL(file);
    }
});

// 글쓰기 제출 수정
document.getElementById('submitPost').addEventListener('click', () => {
    const title = document.getElementById('postTitle').value;
    const author = document.getElementById('postAuthor').value;
    const content = document.getElementById('postContent').value;
    const imageInput = document.getElementById('postImage');
    
    if (!title || !author || !content) {
        alert('제목, 작성자, 내용을 입력해주세요.');
        return;
    }
    
    // 이미지 처리
    let imageData = null;
    if (imageInput.files[0]) {
        const reader = new FileReader();
        reader.onload = function(e) {
            imageData = e.target.result;
            
            // 새 게시글 추가
            addNewPost(title, author, content, imageData);
        };
        reader.readAsDataURL(imageInput.files[0]);
    } else {
        // 이미지 없는 경우
        addNewPost(title, author, content, null);
    }
});

// 게시글 추가 함수
function addNewPost(title, author, content, imageData) {
    // 서버로 POST 요청
    fetch('/posts', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            title: title,
            author: author,
            content: content,
            image: imageData
        })
    })
    .then(response => response.json())
    .then(data => {
        if (data.status === 'success') {
            // 서버 응답 성공시 화면 갱신
            const newPost = {
                id: currentId++,
                title: title,
                author: author,
                content: content,
                image: imageData,
                date: new Date().toLocaleDateString()
            };
            posts.unshift(newPost);
            displayPosts();
            
            // 모달 초기화
            document.getElementById('writeModal').style.display = 'none';
            document.getElementById('postTitle').value = '';
            document.getElementById('postAuthor').value = '';
            document.getElementById('postContent').value = '';
            document.getElementById('postImage').value = '';
            document.getElementById('imagePreview').innerHTML = '';
        }
    })
    .catch(error => {
        console.error('Error:', error);
        alert('게시글 작성에 실패했습니다.');
    });
}

// 게시글 상세보기
function showPostDetail(post) {
    // 이전 모달 제거
    const oldModal = document.getElementById('detailModal');
    if (oldModal) {
        oldModal.remove();
    }

    const detailHTML = `
        <div class="detail-modal" id="detailModal">
            <div class="detail-content">
                <h2>${post.title}</h2>
                <p>작성자: ${post.author}</p>
                <p>작성일: ${post.date}</p>
                ${post.image ? `<img src="${post.image}" alt="게시글 이미지">` : ''}
                <p>${post.content}</p>
                <button onclick="closeModal()">닫기</button>
            </div>
        </div>
    `;
    
    document.body.insertAdjacentHTML('beforeend', detailHTML);
    document.getElementById('detailModal').style.display = 'block';
    
    // ESC 키 이벤트 리스너 추가
    document.addEventListener('keydown', function(e) {
        if (e.key === 'Escape') {
            closeModal();
        }
    });
}

// 모달 닫기 함수
function closeModal() {
    const modal = document.getElementById('detailModal');
    if (modal) {
        modal.remove();
    }
}

// 초기 게시글 목록 표시
displayPosts(); 