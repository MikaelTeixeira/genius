const firebaseConfig = {
    apiKey: "SUA_API_KEY",
    authDomain: "SEU_AUTH_DOMAIN",
    databaseURL: "SUA_DATABASE_URL",
    projectId: "SEU_PROJECT_ID",
    storageBucket: "SEU_STORAGE_BUCKET",
    messagingSenderId: "SEU_SENDER_ID",
    appId: "SEU_APP_ID"
};

firebase.initializeApp(firebaseConfig);

const db = firebase.database();

const rankingRef = db.ref("ranking");

rankingRef
    .orderByChild("pontos")
    .limitToLast(5)
    .on("value", snap => {
        const lista = document.getElementById("ranking-list");
        lista.innerHTML = "";

        let dados = [];

        snap.forEach(child => {
            dados.push(child.val());
        });

        // organiza em ordem decrescente
        dados.sort((a, b) => b.pontos - a.pontos);

        dados.forEach(item => {
            const li = document.createElement("li");
            li.textContent = `${item.nome} — ${item.pontos}`;
            lista.appendChild(li);
        });
    });
