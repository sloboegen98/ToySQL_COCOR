SELECT * 
FROM T1, T2
WHERE T1.x = T1.y + T2.y AND (T1.x = 1 OR T2.x = 1);