Інструкція до програми "Solar System"
Опис програми
Ця програма — симулятор Сонячної системи, створений із використанням бібліотеки OpenGL. Вона моделює рух планет навколо Сонця, їх обертання навколо осі та орбіту супутника (Місяця) навколо Землі. Також на фоні зображені зірки, які додають реалістичності.

Користувач може взаємодіяти з симуляцією через клавіатуру та мишу, змінюючи кут огляду, масштабування та стан анімації.

Основні можливості
Візуалізація Сонячної системи:

Сонце та вісім планет (Меркурій, Венера, Земля, Марс, Юпітер, Сатурн, Уран, Нептун).
Земля має супутник (Місяць), що обертається по орбіті.
Планети рухаються по своїх орбітах, одночасно обертаючись навколо власних осей.
Фон:

Генерація випадкових зірок, що відображаються у тривимірному просторі.
Текстури:

Для кожного тіла Сонячної системи використовуються текстури, що імітують їх вигляд.
Взаємодія з користувачем:

Керування анімацією (зупинка/відновлення).
Зміна кута огляду.
Збільшення/зменшення масштабу сцени.
Інструкція з використання
Запуск програми
Запустіть програму через консоль або подвійним кліком по виконуваному файлу.
Управління
Пробіл (Space): Зупинити/відновити анімацію.
Клавіші W/S: Поворот сцени вверх/вниз.
Клавіші A/D: Поворот сцени вліво/вправо.
Колесо миші:
Прокрутка вгору — збільшення масштабу.
Прокрутка вниз — зменшення масштабу.
Клавіша Q: Вихід із програми.
Вікно програми
Розмір вікна за замовчуванням: 800x600 пікселів. Можна змінювати розмір, програма автоматично підлаштовує перспективу.
Технічні аспекти
Текстури:

Функція LoadTexture завантажує текстури у форматі BMP.
Перетворення кольорових каналів (RGB) здійснюється для правильного відображення.
Анімація:

Швидкість обертання планет навколо Сонця задається масивом orbitSpeeds.
Швидкість обертання навколо осі задається масивом axisSpeeds.
Масштабування:

Масштаб сцени контролюється змінною scaleFactor.
Генерація зірок:

Координати зірок генеруються випадково у межах тривимірного простору.
Фон і освітлення:

Реалізовано систему освітлення через функції OpenGL для створення реалістичного вигляду.
