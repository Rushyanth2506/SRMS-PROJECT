let stack = [];

function updateStack() {
    let stackDiv = document.getElementById("stack");
    stackDiv.innerHTML = "";

    stack.forEach(item => {
        let box = document.createElement("div");
        box.classList.add("box");
        box.textContent = item;
        stackDiv.appendChild(box);
    });
}

function pushElement() {
    let val = document.getElementById("value").value;

    if (val.trim() === "") {
        alert("Enter a value to push!");
        return;
    }

    stack.push(val);
    document.getElementById("value").value = "";
    updateStack();
}

function popElement() {
    if (stack.length === 0) {
        alert("Stack is empty!");
        return;
    }

    let stackDiv = document.getElementById("stack");
    let topBox = stackDiv.firstChild;

    // Pop animation
    topBox.classList.add("pop");

    setTimeout(() => {
        stack.pop();
        updateStack();
    }, 300);
}
