from PuzzleEntity import PuzzleEntity, Color, Endpoint, PathDot, Blob, Star, Triangle, Cancel
from BlockGroup import BlockGroup
from Grid import Grid
from SymmetryGrid import RotationalGrid, HSymmetryGrid, VSymmetryGrid
import Utils
import GridUtils
import math
from PIL import Image, ImageDraw


# Diameter always means the distance across in the cardinal directions.
# If there is an angle then the diameter is rotated around the same angle

def generateHexagon(pos, diameter):
	list = []
	for i in range(6):
		list.append((pos[0] + diameter * 0.5 * math.cos(math.pi * i / 3), pos[1] + diameter * 0.5 * math.sin(math.pi * i / 3)))
	
	return list

def generateTriangle(pos, diameter):
	list = []
	for i in range(3):
		list.append((pos[0] + diameter * 0.5 * math.cos(math.pi * (2 * i / 3 + 1.5)), pos[1] + diameter * 0.5 * math.sin(math.pi * (2 * i / 3 + 1.5))))
	
	return list

def generateBlob(pos, diameter):
	list = []
	dx = [1, -1, -1, 1]
	dy = [1, 1, -1, -1]
	for i in range(8):
		xp = (pos[0] + diameter * 0.5 * dx[i % 4])
		yp = (pos[1] + diameter * 0.5 * dy[i % 4])
		list.append((xp, yp))
	return list

def generateStar(pos, diameter):
	list = [[], []]
	dx = [1, 0, -1, 0]
	dy = [0, 1, 0, -1]
	for i in range(4):
		xp = (pos[0] + diameter * 0.5 * dx[i % 4])
		yp = (pos[1] + diameter * 0.5 * dy[i % 4])
		list[0].append((xp, yp))

	dx = [1, -1, -1, 1]
	dy = [1, 1, -1, -1]
	for i in range(4):
		xp = (pos[0] + diameter * math.sqrt(0.125) * dx[i % 4])
		yp = (pos[1] + diameter * math.sqrt(0.125) * dy[i % 4])
		list[1].append((xp, yp))

	return list

def generateSquare(pos, diameter, angle):
	list = []
	for i in range(4):
		theta = math.pi * (0.25 + (i / 2)) + angle
		xp = pos[0] + (diameter * 0.5) * math.cos(theta)
		yp = pos[1] + (diameter * 0.5) * math.sin(theta)
		list.append((xp, yp))
	return list

def generateBlockGroup(pos, element, block_spacing, angle, block_scale = 0.95):
	element.normalize()
	list = []
	if not isinstance(element, BlockGroup):
		return list
	
	bb = Utils.dims(element.boundingbox)
	s = math.sin(angle)
	c = math.cos(angle)
	
	for block in element.points:
		xp = (block[0] - (bb[0] - 1) / 2) * block_spacing
		yp = (block[1] - (bb[1] - 1) / 2) * block_spacing

		xpos = pos[0] + xp * c - yp * s
		ypos = pos[1] - xp * s - yp * c
		list.append(generateSquare((xpos, ypos), block_spacing * block_scale, -1 * angle))
	
	return list

# Right now bg, line, path are solid colors. Filter is a solid color that dictates the light filter.

def diminish(color, filter):
	return ((color[0] * filter[0]) // 255, (color[1] * filter[1]) // 255, (color[2] * filter[2]) // 255)

def multiply(color, scale):
	return (int(color[0] * scale), int(color[0] * scale), int(color[0] * scale))

def colorize(grid, pos, filter, violations, line_cols = None):
	element = grid.get(pos)
	
	# print(element, " / ", grid.violations)
	if (element in violations or pos in violations):
		return Color.RGB_RED.value if (element.color != Color.RGB_RED) else (127, 0, 0)
	
	# Special case for symmetry dots
	if Utils.instanceof(element, PathDot):
		if element.restriction != 0:
			if line_cols == None:
				G = 1
				R = element.restriction % 2
				B = (element.restriction + 1) % 2
				col = (R * 255, G * 255, B * 255)
				return diminish(col, filter)
			else:
				restr = element.restriction
				count = 0
				while (restr % 2 == 0 and restr > 0):
					count += 1
					restr //= 2
				return diminish(line_cols[count % len(line_cols)], filter)

	if (element.color == Color.NIL):
		return 0
	
	return diminish(element.color.value, filter)

def render(output, grid: Grid, width = 1024, height = 1024, margin = 96, thickness = 48, bg_ = (128, 128, 128), path_ = (64, 64, 64), line = [(255, 255, 255), (0, 255, 255), (255, 0, 255), (255, 255, 0)], filter = (255, 255, 255), getViolations = False):
	puzzle = Grid(grid.R, grid.C)
	

	for r in range(grid.R):
		for c in range(grid.C):
			puzzle.board[r][c] = grid.board[c][grid.R - r - 1] # Rotate the grid so it renders where [0][0] is bottom left and the first and second indices are x and y axes
	
	violations = set()

	violations2 = GridUtils.getViolations(grid)
	if getViolations:
		for p in violations2:
			violations.add((p[1], grid.R - p[0] - 1))

	
	# Set colors
	bg = diminish(bg_, filter)
	line_colors = []
	for i in range(len(line)):
		line_colors.append(diminish(line[i], filter))
	path = diminish(path_, filter)

	image = Image.new("RGB", (width, height), bg)
	
	# Number of pixels between grid ranks and files (including symbols and paths)

	grid_height = (width - 2 * margin) / (len(puzzle.board) - 1)
	grid_width = (height - 2 * margin) / (len(puzzle.board[0]) - 1)

	grid_spacing = min(grid_width, grid_height)
	margin_x = (width - grid_spacing * (len(puzzle.board[0]) - 1)) / 2
	margin_y = (height - grid_spacing * (len(puzzle.board) - 1)) / 2

	# Constants

	length = margin / 2 # Length of endpoint protrusions
	rad = thickness / 2

	draw = ImageDraw.Draw(image)

	# Draw the lines

	for i in range(0, len(puzzle.board), 2):
		a = (margin_x, margin_y + grid_spacing * i)
		b = (width - margin_x, margin_y + grid_spacing * i)
		draw.line([a, b], width=thickness, fill=path, joint="curve")
	
	for i in range(0, len(puzzle.board[0]), 2):
		a = (margin_x + grid_spacing * i, margin_y)
		b = (margin_x + grid_spacing * i, height - margin_y)
		draw.line([a, b], width=thickness, fill=path, joint="curve")

	# Draw the ending points

	for i in range(len(puzzle.board)):
		for j in range(len(puzzle.board[i])):
			xpos = margin_x + grid_spacing * j
			ypos = margin_y + grid_spacing * i
			element = puzzle.get((i, j))
			
			if Utils.instanceof(element, Endpoint) and (not element.isStart):
				if (i == 0 and j == 0):
					draw.line((xpos - length, ypos - length, xpos, ypos), width=thickness, fill=path, joint="curve")
					draw.ellipse((xpos - length - rad, ypos - length - rad, xpos - length + rad, ypos - length + rad), fill=path)
				elif (i == 0 and j == len(puzzle.board[i]) - 1):
					draw.line((xpos + length, ypos - length, xpos, ypos), width=thickness, fill=path, joint="curve")
					draw.ellipse((xpos + length - rad, ypos - length - rad, xpos + length + rad, ypos - length + rad), fill=path)
				elif (i == len(puzzle.board) - 1 and j == len(puzzle.board[i]) - 1):
					draw.line((xpos + length, ypos + length, xpos, ypos), width=thickness, fill=path, joint="curve")
					draw.ellipse((xpos + length - rad, ypos + length - rad, xpos + length + rad, ypos + length + rad), fill=path)
				elif (i == len(puzzle.board) - 1 and j == 0):
					draw.line((xpos - length, ypos + length, xpos, ypos), width=thickness, fill=path, joint="curve")
					draw.ellipse((xpos - length - rad, ypos + length - rad, xpos - length + rad, ypos + length + rad), fill=path)
				elif (i == 0):
					draw.line((xpos, ypos - length, xpos, ypos), width=thickness, fill=path, joint="curve")
					draw.ellipse((xpos - rad, ypos - length - rad, xpos + rad, ypos - length + rad), fill=path)
				elif (i == len(puzzle.board) - 1):
					draw.line((xpos, ypos + length, xpos, ypos), width=thickness, fill=path, joint="curve")
					draw.ellipse((xpos - rad, ypos + length - rad, xpos + rad, ypos + length + rad), fill=path)
				elif (j == 0):
					draw.line((xpos - length, ypos, xpos, ypos), width=thickness, fill=path, joint="curve")
					draw.ellipse((xpos - length - rad, ypos - rad, xpos - length + rad, ypos + rad), fill=path)
				elif (j == len(puzzle.board[i]) - 1):
					draw.line((xpos, ypos, xpos + length, ypos), width=thickness, fill=path, joint="curve")
					draw.ellipse((xpos + length - rad, ypos - rad, xpos + length + rad, ypos + rad), fill=path)
				
	# Draw the rounded corners

	for i in range(0, len(puzzle.board), 2):
		for j in range(0, len(puzzle.board[i]), 2):
			xpos = margin_x + grid_spacing * j
			ypos = margin_y + grid_spacing * i
			draw.ellipse((xpos - rad + 1, ypos - rad + 1, xpos + rad, ypos + rad), fill=path)
			if (Utils.instanceof(puzzle.get((i, j)), Endpoint)) and puzzle.get((i, j)).isStart:
				draw.ellipse((xpos - thickness, ypos - thickness, xpos + thickness, ypos + thickness), fill=path)
	
	for i in range(0, len(puzzle.board)):
		for j in range(0, len(puzzle.board[i])):
			xpos = margin_x + grid_spacing * j
			ypos = margin_y + grid_spacing * i

			element = puzzle.get((i, j))

			cut_radius = max(grid_spacing - thickness * 1.5, thickness * 0.5)
			# Draw cuts in the path
			if (i % 2 == 0 or j % 2 == 0) and not element.isPath:
				# print((i, j))
				draw.rectangle((xpos - cut_radius, ypos - cut_radius, xpos + cut_radius, ypos + cut_radius), fill=bg)

			# Draw dots on the path
			if (isinstance(element, PathDot)):
				hex = generateHexagon((xpos, ypos), thickness * 0.9)
				color = puzzle.board[i][j].color
				draw.polygon(hex, fill = colorize(puzzle, (i, j), filter, violations, line_colors))

				puzzle.board[i][j].color = color
			
			# Draw blobs
			if (isinstance(element, Blob)):
				shape = generateBlob((xpos, ypos), grid_spacing / 4)
				# print(shape)
				draw.line(shape, width = int(grid_spacing) // 2, fill = colorize(puzzle, (i, j), filter, violations), joint="curve")
			
			# Draw stars
			if (isinstance(element, Star)):
				shape = generateStar((xpos, ypos), grid_spacing * 2/3)
				# print(shape)
				draw.polygon(shape[0], fill = colorize(puzzle, (i, j), filter, violations))
				draw.polygon(shape[1], fill = colorize(puzzle, (i, j), filter, violations))
			
			# Draw triangles

			if (isinstance(element, Triangle)):
				for ii in range(element.count):
					xpos2 = xpos + (ii + 0.5 - element.count / 2) * (grid_spacing / 2)
					shape = generateTriangle((xpos2, ypos), grid_spacing / 2)
					draw.polygon(shape, fill = colorize(puzzle, (i, j), filter, violations))
			
			# Draw blocks

			if (isinstance(element, BlockGroup)):
				angle = 0 if element.fixed else math.pi / 6
				dim = Utils.dims(element.boundingbox)
				dimension = max(dim[0], dim[1])

				shape = generateBlockGroup((xpos, ypos), element, grid_spacing / dimension, angle)
				for s in shape:
					draw.polygon(s, fill = colorize(puzzle, (i, j), filter, violations))
				
				if element.value < 0:
					shape = generateBlockGroup((xpos, ypos), element, grid_spacing / dimension, angle, 0.7)
				
					for s in shape:
						draw.polygon(s, fill = diminish(bg, filter))

			# Draw cancels

			if (isinstance(element, Cancel)):
				crad = grid_spacing / 3
				linewidth = thickness // 2
				color = colorize(puzzle, (i, j), filter, violations)
				if (element.color == Color.NIL):
					color = filter
				draw.line((xpos, ypos - crad, xpos, ypos), width = linewidth, fill = color)
				draw.line((xpos - crad * math.sqrt(0.75), ypos + crad * 0.5, xpos, ypos), width = linewidth, fill = color)
				draw.line((xpos + crad * math.sqrt(0.75), ypos + crad * 0.5, xpos, ypos), width = linewidth, fill = color)

			# Draw the path
			if (element.hasLine != 0):
				LINECOL = line_colors[(element.hasLine - 1) % len(line_colors)]
				if (Utils.instanceof(element, Endpoint) and element.isStart):
					draw.ellipse((xpos - thickness, ypos - thickness, xpos + thickness, ypos + thickness), fill=LINECOL)
				elif Utils.instanceof(element, Endpoint) and (not element.isStart):
					draw.ellipse((xpos - rad, ypos - rad, xpos + rad, ypos + rad), fill=LINECOL)
					if (i == 0 and j == 0):
						draw.line((xpos - length, ypos - length, xpos, ypos), width=thickness, fill=LINECOL, joint="curve")
						draw.ellipse((xpos - length - rad, ypos - length - rad, xpos - length + rad, ypos - length + rad), fill=LINECOL)
					elif (i == 0 and j == len(puzzle.board[i]) - 1):
						draw.line((xpos + length, ypos - length, xpos, ypos), width=thickness, fill=LINECOL, joint="curve")
						draw.ellipse((xpos + length - rad, ypos - length - rad, xpos + length + rad, ypos - length + rad), fill=LINECOL)
					elif (i == len(puzzle.board) - 1 and j == len(puzzle.board[i]) - 1):
						draw.line((xpos + length, ypos + length, xpos, ypos), width=thickness, fill=LINECOL, joint="curve")
						draw.ellipse((xpos + length - rad, ypos + length - rad, xpos + length + rad, ypos + length + rad), fill=LINECOL)
					elif (i == len(puzzle.board) - 1 and j == 0):
						draw.line((xpos - length, ypos + length, xpos, ypos), width=thickness, fill=LINECOL, joint="curve")
						draw.ellipse((xpos - length - rad, ypos + length - rad, xpos - length + rad, ypos + length + rad), fill=LINECOL)
				elif (i % 2 == 0 and j % 2 == 0):
					draw.ellipse((xpos - rad + 1, ypos - rad + 1, xpos + rad, ypos + rad), fill=LINECOL)
				elif (j % 2 == 1):
					draw.line((xpos - grid_spacing, ypos, xpos + grid_spacing, ypos), fill=LINECOL, width=thickness)
				elif (i % 2 == 1):
					draw.line((xpos, ypos - grid_spacing, xpos, ypos + grid_spacing), fill=LINECOL, width=thickness)

	image.save(output)
	image.close()