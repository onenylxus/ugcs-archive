/*
  CSCI2720 Assignment 1
  Ng Wing Yin 1155127101
*/

// Show hidden header by special button toggle
const hiddenHeader = document.querySelector('.hidden-header');
const specialButton = document.querySelector('.nav-special');
specialButton.addEventListener('click', () => {
  hiddenHeader.classList.toggle('show');
});

// Hidden buttons
const [align, newHobby, progress] = [...document.querySelectorAll('.hidden-button')];

// Align section headings
align.addEventListener('click', () => {
  document.querySelectorAll('.section-heading').forEach((heading) => {
    console.log(heading);
    switch (true) {
      case heading.classList.contains('text-start'):
        heading.classList.replace('text-start', 'text-center');
        break;
        
      case heading.classList.contains('text-center'):
        heading.classList.replace('text-center', 'text-end');
        break;

      case heading.classList.contains('text-end'):
        heading.classList.replace('text-end', 'text-start');
        break;

			default:
        heading.classList.add('text-center');
		}
  });
});

// Prompt and add new hobby
newHobby.addEventListener('click', () => {
  const hobby = prompt('Enter a new hobby:');
  if (hobby.length > 0) {
    const div = document.createElement('div');
		div.classList.add('hobbies-item');
		div.innerText = hobby;
		document.getElementById('hobby-list').appendChild(div);
  }
});

// Scroll progress
progress.addEventListener('click', () => {
  document.querySelector('.progress').classList.toggle('show');
});
window.addEventListener('scroll', () => {
  const bar = document.querySelector('.progress-bar')
  const pc = (window.scrollY / (document.body.clientHeight - window.innerHeight)) * 100;
  bar.setAttribute('aria-valuenow', pc);
  bar.style.width = `${pc}%`;
});

// Resize canvas
const canvas = document.querySelector('canvas');
const image = document.querySelector('.home-profile-image');
canvas.width = image.width;
canvas.height = image.height;

// Particle class
class Particle {
  constructor() {
    this.x = Math.random() * 500;
    this.y = Math.random() * 500;
    this.r = 3 + Math.random() * 2;
    this.dx = Math.random() - 0.5;
    this.dy = 1 + Math.random() * 2;
    this.a = 0;
    this.c = ['#013026', '#014760', '#107e57', '#a1ce3f', '#cbe58e'][Math.floor(Math.random() * 4.99)];
  }

  get valid() {
    return this.x >= 0 && this.x <= 500 && this.y <= 500 && this.alpha >= 0;
  }

  get alpha() {
    return Math.sin(this.a / 15);
  }

  update(context) {
    this.a++;
    this.x += this.dx;
    this.y += this.dy;

    context.globalAlpha = this.alpha;
    context.fillStyle = this.c;
    context.beginPath();
    context.arc(this.x, this.y, this.r, 0, Math.PI * 2);
    context.fill();
  }
}

// Canvas animation
const context = canvas.getContext('2d');
let particles = [];
setInterval(() => particles.push(new Particle()), 1);
function draw() {
  requestAnimationFrame(draw);
  context.clearRect(0, 0, canvas.width, canvas.height);
  particles = particles.filter((p) => p.valid);
  particles.forEach((p) => p.update(context));
}
draw();

// Check comment form submission
function processform() {
  const email = document.getElementById('new-email');
	const comment = document.getElementById('new-comment');
	email.classList[email.validity.valid ? 'remove' : 'add']('is-invalid');
	comment.classList[comment.validity.valid ? 'remove' : 'add']('is-invalid');

  if (email.validity.valid && comment.validity.valid && [...document.querySelectorAll('.form-check-input')].some((radio) => radio.checked)) {
    appendComment(email, comment);
  }
}

function appendComment(email, comment) {
  const newComment = document.createElement('div');
  const element = '<div><svg height="100" width="100"><circle cx="50" cy="50" r="40"></svg></div><div><h5></h5><p></p></div>';
  newComment.innerHTML = element;

  newComment.className = 'd-flex';
	newComment.querySelectorAll('div')[0].className = 'flex-shrink-0'; // 1st div
	newComment.querySelectorAll('div')[1].className = 'flex-grow-1'; // 2nd div

  const lastComment = document.querySelector('#comments').lastElementChild; // instead of lastChild for div element
	newComment.id = `c${Number(lastComment.id.substr(1)) + 1}`;

  newComment.querySelector('h5').innerHTML = email.value;
	newComment.querySelector('p').innerHTML = comment.value;

  const color = document.querySelectorAll('input[name=new-color]:checked')[0].value;
  newComment.querySelector('circle').setAttribute('fill', color);

  document.querySelector('#comments').appendChild(newComment);
  document.querySelector('form').reset();

  saveComments();
}

// Save comments
function saveComments() {
	fetch('./comments.txt', {
		method: 'PUT',
		body: document.getElementById('comments').innerHTML
	});
}

// Load comments
function loadComments() {
	fetch('comments.txt').then((res) => res.text()).then((txt) => { document.getElementById('comments').innerHTML = txt; });
}

// Reset comments (debug use)
function resetComments() {
  fetch('./comments.txt', {
		method: 'PUT',
		body: `<div id="c1001" class="d-flex">
            <div class="flex-shrink-0">
              <svg height="100" width="100">
                <circle cx="50" cy="50" r="40" fill="green">
                </circle>
              </svg>
            </div>
            <div class="flex-grow-1">
              <h5>chuckjee@cse.cuhk.edu.hk</h5>
              <p>This lab is so fun! Let's explore further with Bootstrap and JS fetch.</p>
            </div>
          </div>
          `
	}).then(() => loadComments());
}

// Load when this defer script is called
loadComments();