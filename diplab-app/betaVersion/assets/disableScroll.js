var webpage = document.getElementById('webpage');

webpage.addEventListener('wheel', function(e) {
    e.preventDefault();
}, { passive: false });

webpage.addEventListener('touchmove', function(e) {
    e.preventDefault();
}, { passive: false });