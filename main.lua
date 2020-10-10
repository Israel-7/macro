local k = require("vkeys")

function main()
	iterations = 1
	shutdown = false
	----
	
	fails = 0
	
	logFile = io.open("log.log", "w")
	
	writeLog = function(text)
		logFile:write(os.date("[%d/%m/%Y %H:%M:%S] ") .. text .. "\n")
	end
	
	total = os.time()
	
	writeLog("Macro iniciado")
	writeLog(iterations .. " DG(s)")
	
	if shutdown then
		writeLog("Desligamento ON\n")
	else
		writeLog("Desligamento OFF\n")
	end
	
	logFile:write("--------------------------------------------------------------------------------\n")

	for n = 1, iterations do
		currentDG = n
		
		if not findImage("buff.jpg", false, 1321, 176, 44, 235) then
			writeLog("Buff Lamina Esmagadora nao encontrado")
			
			rightClick(420, 645)
			wait(1100)
			
			writeLog("Buff Lamina Esmagadora renovado")
		end
		
		if not findImage("bless3.jpg", false, 1018, 24, 217, 62) and not findImage("bless4.jpg", false, 1018, 24, 217, 62) then
			writeLog("Bencao do GM nao encontrada")
			
			keyPress(k.VK_O)
			wait(100)
			findImage("selectServer.jpg", true, 603, 301, 160, 78)
			click()
			wait(100)
			findImage("yes.jpg", true, 606, 392, 172, 76)
			click()
			
			writeLog("Relogando...")
			
			repeat
				wait(80)
			until findImage("connect.jpg", false, 501, 630, 218, 63)
			
			keyPress(k.VK_RETURN)
			
			repeat
				wait(80)
			until findImage("start.jpg", false, 1071, 569, 133, 46)
			
			keyPress(k.VK_RETURN)
			wait(700)
			
			if findImage("subPass.jpg", false, 534, 267, 295, 68) then
				writeLog("Sub-Senha solicitada")
				
				findImage("two.jpg", true, 690, 335, 113, 90)
				click()
				wait(500)
				click()
				findImage("four.jpg", true, 690, 335, 113, 90)
				click()
				wait(500)
				click()
				wait(500)
				findImage("ok.jpg", true, 643, 394, 197, 93)
				click()
			end
			
			writeLog("Conectando...")
			
			repeat
				wait(80)
			until findImage("bless3.jpg", false, 1018, 24, 217, 62) or findImage("bless4.jpg", false, 1018, 24, 217, 62)	

			writeLog("Conectado")
		end
		
		wait(300)
		
		if findImage("special.jpg", false, 517, 561, 198, 60) then
			keyPress(k.VK_ESCAPE)
			writeLog("Inventario especial rejeitado")
		end
		
		keyPress(k.VK_7)
		
		repeat
			wait(80)
			click(710, 263)
		until findImage("enterDG.jpg", true, 516, 516, 335, 63)
		
		click()
		writeLog("Entrando na DG...\n")
		
		repeat
			wait(80)
		until findImage("challenge.jpg", true, 493, 524, 160, 75)
		
		limit = os.time() + 240
		timer = os.time()
		DGFailed = false
		
		click()
		writeLog("DG " .. currentDG .. " de " .. iterations .. " iniciada")
		
		wait(1500)	
		move(793, 268)
		wheel(0, 0, -150)
		wait(700)
		
		if not DGFailed then writeLog("A caminho do Peryton Perfurante...") end
		
		runTo({1116, 319, 1149, 401, 1166, 553, 960, 632, 699, 720, 736, 724, 539, 711, 506, 334})
		findTarget("peryton")
		kill("mob")

		if not DGFailed then writeLog("A caminho do Meca Gorila Defensor T-2...") end
		
		runTo({544, 697, 327, 520, 162, 413, 250, 555, 250, 556})
		findTarget("gorila")
		kill("mob")
		keyPress(k.vk_7)
		
		if not DGFailed then writeLog("A caminho do Primeiro Cavaleiro...") end
		
		runTo({240, 423, 507, 150, 452, 147, 496, 146, 368, 271, 436, 675, 386, 657, 442, 611, 451, 636, 759, 693, 863, 693, 1054, 555, 1093, 510, 1009, 533, 766, 703, 766, 703, 890, 694, 882, 605, 882, 605})
		findTarget("cavaleiro")
		kill("mob")
	
		if not DGFailed then writeLog("A caminho do Primeiro Cavaleiro (II)...") end
		
		runTo({457, 138, 457, 138, 609, 102, 609, 102, 400, 130, 184, 275, 177, 268, 237, 183, 237, 183, 166, 245, 166, 245, 163, 256, 234, 249})
		findTarget("cavaleiro")
		kill("mob")
		keyPress(k.VK_7)
		
		if not DGFailed then writeLog("A caminho do Golem de Carne Wurger...") end
		
		runTo({1058, 698, 1058, 698, 1104, 688, 1148, 656, 1059, 667, 453, 572, 555, 701, 659, 721, 387, 658, 218, 393, 218, 393, 255, 259})
		findTarget("golem")
		kill("mob")

		if not DGFailed then writeLog("A caminho do Patren...") end
		
		runTo({1124, 420, 1124, 420, 1124, 420, 1124, 420, 1119, 441})
		findTarget("patren")
		kill("boss")
		
		findTarget("bau")
		kill("unknown")
		
		if not DGFailed then
			writeLog("Recebendo drops...")
			
			for i = 1, 40 do
				wait(25)
				keyPress(k.VK_SPACE)
			end
			
			repeat
				wait(80)
			until findImage("endDG.jpg", false, 1042, 400, 323, 193)
			
			writeLog("Guardando itens no armazem...")
			
			keyPress(k.VK_I)
			click(1124, 386)
			rightClick(1318, 636)
			keyDown(k.VK_LCONTROL)
			click(1122, 422)
			click(1153, 422)
			click(1183, 422)
			click(1213, 422)
			click(1244, 422)
			click(1275, 422)
			click(1306, 422)
			click(1336, 422)
			keyUp(k.VK_LCONTROL)
			keyPress(k.VK_ESCAPE)
			
			writeLog("DG " .. currentDG .. " de " .. iterations .. " finalizada em " .. os.date("%M:%S", os.time() - timer) .. " minutos")
			
			while findImage("close.jpg", true, 128, 351, 78, 257) do
				click()
			end
			
			findImage("closeDG.jpg", true, 1261, 391, 104, 191)
			click()
			
			repeat
				wait(80)
			until findImage("lot.jpg", true, 617, 543, 131, 37)
			
			click()
			
			repeat
				wait(80)
			until findImage("ok2.jpg", true, 599, 522, 165, 74)
			
			wait(250)
			click()
		end
		
		logFile:write("--------------------------------------------------------------------------------\n")
		wait(3000)
	end
	
	writeLog(iterations - fails .. " DG(s) finalizadas com sucesso")
	writeLog(fails .. " DG(s) canceladas")
	writeLog("Tempo total: " .. os.date("%H:%M:%S", os.time() - total))
	
	if shutdown then
		writeLog("O computador sera desligado em 20 segundos")
		os.execute("shutdown -s -t 20")
	end
	
	logFile:close()
end

function process()
	if not DGFailed then
		if os.time() >= limit then
			click(913, 692)
			click(953, 385)
			click(1162, 443)
			click(675, 440)
			
			DGFailed = true
			fails = fails + 1
			
			writeLog("DG " .. currentDG .. " de " .. iterations .. " cancelada apos ultrapassar o tempo limite")
			return false
		end
		
		if wasKeyPressed(k.VK_TAB) then
			repeat
				wait(0)
			until wasKeyPressed(k.VK_TAB)
		end
		
		return true
	end
	
	return false
end

function runTo(coords)
	local flag = true
	
	for i = 1, #coords, 2 do
		if flag then
			repeat
				if not process() then return end
			until findImage("deslizar.jpg", false, 718, 668, 40, 42)
		else
			repeat
				if not process() then return end
			until findImage("recuar.jpg", false, 679, 671, 38, 39)
		end
		
		move(coords[i], coords[i + 1])
		wait(80)
		keyPress(flag and k.VK_9 or k.VK_8)
		
		flag = not flag
	end
	
	wait(500)
end

function findTarget(target)
	if not DGFailed then writeLog("Buscando alvo...") end
	local counter = 0
	
	repeat
		if not process() then return end
		
		if counter >= 30 then
			click(913, 692)
			click(953, 385)
			click(1162, 443)
			click(675, 440)
			
			DGFailed = true
			fails = fails + 1
			
			writeLog("Alvo nao encontrado apos 30 tentativas")
			writeLog("DG " .. currentDG .. " de " .. iterations .. " cancelada em " .. os.date("%M:%S", os.time() - timer) .. " minutos")
		end
		
		keyPress(k.VK_Z)

		wait(100)
		counter = counter + 1
	until findImage(target .. ".jpg", false, 535, 30, 189, 33)
	
	writeLog("Alvo encontrado apos " .. counter .. " tentativa(s)")
end

function kill(targetType)
	if not DGFailed then writeLog("Atacando...") end
	
	repeat
		if not process() then return end
		keyPress(k.VK_1)
		keyPress(k.VK_2)
		keyPress(k.VK_3)
		keyPress(k.VK_5)
	until not findImage(targetType .. ".jpg", false, 508, 23, 47, 49)
end