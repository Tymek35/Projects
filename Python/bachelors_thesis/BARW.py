import matplotlib.pyplot as plt
import numpy as np
import random
import os
import math
from PIL import Image
from scipy.optimize import curve_fit

T = 100 # Liczba kroków czasowych
b_p = 0.15 # Prawdopodobieństwo rozgałęzienia
l = 1 # Długość przedłużenia
a_e = np.pi / 10 # Zakres kątów przy przedłużeniu w radianach
a_b_b = np.pi / 10 # Zakres kątów przy rozgałęzieniu dolna granica
a_b_u = np.pi / 2 # Zakres kątów przy rozgałęzieniu górna granica
r_a = (np.sqrt(5) / 2) * l # Promień anihilacji
forces = False # Czy symulacja uwzględnia siły
radial = False # Czy pole zewnętrzne radialne
homogeneous = False # Czy pole zewnętrzne jednorodne
double_radial = False # Czy pole zewnętrzne to złożenie dwóch pól radialnych
f_e = 0 # Natężenie pola zewnętrznego
f_c = 0 # Natężenie sił odpychających
r_c = 3 * l # Zasięg sił odpychania
second_radial_x = 100 * l # Współrzędna x centrum drugiego pola radialnego
second_radial_y = 0 * l # Współrzędna y centrum drugiego pola radialnego
seed = 1 # Seed generatora liczb losowych

# Węzeł drzewa binarnych wyszukiwań
class Node:
	def __init__(self, point):
		self.left = None
		self.right = None
		self.val = point
		self.backup_array = []

# Wstawienie do drzewa binarnych wyszukiwań
def insert(root, point):
	if root is None:
		return Node(point)
	else:
		if root.val[0] == point[0]:
			root.backup_array.append(point[0])
			root.backup_array.append(point[1])
			return root
		elif root.val[0] < point[0]:
			root.right = insert(root.right, point)
		else:
			root.left = insert(root.left, point)
	return root

# Zwraca wszystkie punkty w zakresie współrzędnej
def get_all_in_range(root, x_min, x_max, array):
	if root is None:
		return
		
	if x_min < root.val[0]:
		get_all_in_range(root.left, x_min, x_max, array)
	
	if root.val[0] >= x_min and root.val[0] <= x_max:
		array.append(root.val)
		if len(root.backup_array) != 0:
			for p in root.backup_array:
				array.append(p)
	
	get_all_in_range(root.right, x_min, x_max, array)

# Przepisz wszystkie punkty z drzewa do tablicy
def get_nodes_as_array(root, nodes):
	if root is None:
		return
	
	get_nodes_as_array(root.left, nodes)
	nodes.append(root)
	get_nodes_as_array(root.right, nodes)

# Zbuduj drzewo z posortowanej tablicy
def build_tree(nodes, start, end):
	if start > end:
		return None
	
	mid = (start + end) // 2
	node = nodes[mid]
	node.left = build_tree(nodes, start, mid - 1)
	node.right = build_tree(nodes, mid + 1, end)
	return node

# Funkcja tworzy zbalansowane bst
def balance_tree(root):
	nodes = []
	get_nodes_as_array(root, nodes)
	n = len(nodes)
	return build_tree(nodes, 0, n - 1)

# Zwraca najbliższy punkt
def get_closest(points, point):
	closest = points[0]
	closest_distance = np.sqrt((point[0] - closest[0]) * (point[0] - closest[0]) + (point[1] - closest[1]) * (point[1] - closest[1]))
	for p in points:
		distance = np.sqrt((point[0] - p[0]) * (point[0] - p[0]) + (point[1] - p[1]) * (point[1] - p[1]))
		if distance < closest_distance:
			closest = p
			closest_distance = distance
	
	return closest

# Znajdź wszystkie punkty w promieniu od proponowanego punktu poza poprzednimi dwoma punktami w tej ścieżce
def get_points_in_radius_for_point(active_paths, all_points, path_index, r, point):
	points_in_range = []
	points_in_radius = []
	last_point = active_paths[path_index][len(active_paths[path_index]) - 1]
	penultimate_point = []
	if len(active_paths[path_index]) > 1:
		penultimate_point = active_paths[path_index][len(active_paths[path_index]) - 2]
	get_all_in_range(all_points, point[0] - r, point[0] + r, points_in_range)
	for p in points_in_range:
		distance = np.sqrt((point[0] - p[0]) * (point[0] - p[0]) + (point[1] - p[1]) * (point[1] - p[1]))
		if distance < r and p[0] != point[0] and p[1] != point[1] and p[0] != last_point[0] and p[1] != last_point[1]:
			if len(penultimate_point) == 0 or (p[0] != penultimate_point[0] and p[1] != penultimate_point[1]):
				points_in_radius.append(p)
	
	return points_in_radius

# Znajdź wszystkie punkty w promieniu od ostatniego punkty w aktywnej ścieżce poza poprzednim punktem w tej ścieżce
def get_points_in_radius(active_paths, all_points, path_index, r, point=[0, 0]):
	points_in_range = []
	points_in_radius = []
	last_point = active_paths[path_index][len(active_paths[path_index]) - 1]
	penultimate_point = active_paths[path_index][len(active_paths[path_index]) - 2]
	get_all_in_range(all_points, last_point[0] - r, last_point[0] + r, points_in_range)
	for p in points_in_range:
		distance = np.sqrt((last_point[0] - p[0]) * (last_point[0] - p[0]) + (last_point[1] - p[1]) * (last_point[1] - p[1]))
		if distance < r and p[0] != penultimate_point[0] and p[1] != penultimate_point[1] and p[0] != last_point[0] and p[1] != last_point[1]:
			points_in_radius.append(p)
	
	return points_in_radius

# Pole jednorodne
def homogeneous_field():
	return [f_e, 0]
	
# Pole radialne
def radial_field(active_point):
	length = np.sqrt(active_point[0] * active_point[0] + active_point[1] * active_point[1])
	force = [active_point[0] * f_e / length, active_point[1] * f_e / length]
	return force

# Suma dwóch pól radialnych
def double_radial_field(active_point):
	length_1 = np.sqrt(active_point[0] * active_point[0] + active_point[1] * active_point[1])
	force_1 = [active_point[0] * f_e / length_1, active_point[1] * f_e / length_1]
	vector_2 = [second_radial_x - active_point[0], second_radial_y - active_point[1]]
	length_2 = np.sqrt(vector_2[0] * vector_2[0] + vector_2[1] * vector_2[1])
	force_2 = [vector_2[0] * f_e / length_2, vector_2[1] * f_e / length_2]
	force = [force_1[0] + force_2[0], force_1[1] + force_2[1]]
	length = np.sqrt(force[0] * force[0] + force[1] * force[1])
	return [force[0] * f_e / length, force[1] * f_e / length]

# Przesunięcie punktu aktywnego w wyniku sił
def displacement(active_paths, all_points, path_index, active_point):
	external_force = [0, 0]
	if homogeneous:
		external_force = homogeneous_field()
	elif radial:
		external_force = radial_field(active_point)
	elif double_radial:
		external_force = double_radial_field(active_point)
	
	if f_c != 0:
		points_in_radius = get_points_in_radius_for_point(active_paths, all_points, path_index, r_c, active_point)
		if r_c > l:
			points_in_radius.append(active_paths[path_index][len(active_paths[path_index]) - 2])
		for point in points_in_radius:
			force = [active_point[0] - point[0], active_point[1] - point[1]]
			length = np.sqrt(force[0] * force[0] + force[1] * force[1])
			force[0] = force[0] * f_c / length
			force[1] = force[1] * f_c / length
			external_force[0] = external_force[0] + force[0]
			external_force[1] = external_force[1] + force[1]
	
	active_point[0] = active_point[0] + external_force[0]
	active_point[1] = active_point[1] + external_force[1]
	last_point = active_paths[path_index][len(active_paths[path_index]) - 1]
	vector = [active_point[0] - last_point[0], active_point[1] - last_point[1]]
	length = np.sqrt(vector[0] * vector[0] + vector[1] * vector[1])
	vector[0] = vector[0] * l / length
	vector[1] = vector[1] * l / length
	return [last_point[0] + vector[0], last_point[1] + vector[1]]

# Zwraca kierunek wyznaczony przez dwa ostatnie punkty w ścieżce
def get_simple_direction(active_paths, all_points, path_index):
	path_last_index = len(active_paths[path_index]) - 1
	return [active_paths[path_index][path_last_index][0] - active_paths[path_index][path_last_index - 1][0], active_paths[path_index][path_last_index][1] - active_paths[path_index][path_last_index - 1][1]]

# Sprawdza czy punkt jest punktem aktywnym
def check_if_active(point, active_paths):
	for path in active_paths:
		i = len(path) - 1
		if point[0] == path[i][0] and point[1] == path[i][1]:
			return True
			
	return False
	
# Rozgałęzienia
def branch(active_paths, all_points, inactive_paths, path_index):
	angle_1 = random.uniform(a_b_b, a_b_u)
	angle_2 = -random.uniform(a_b_b, a_b_u)
	path_last_index = len(active_paths[path_index]) - 1
	direction = get_simple_direction(active_paths, all_points, path_index)
	branch_1_vector = [direction[0] * np.cos(angle_1) - direction[1] * np.sin(angle_1), direction[0] * np.sin(angle_1) + direction[1] * np.cos(angle_1)]
	branch_2_vector = [direction[0] * np.cos(angle_2) - direction[1] * np.sin(angle_2), direction[0] * np.sin(angle_2) + direction[1] * np.cos(angle_2)]
	branch_1_point = [active_paths[path_index][path_last_index][0] + branch_1_vector[0], active_paths[path_index][path_last_index][1] + branch_1_vector[1]]
	branch_2_point = [active_paths[path_index][path_last_index][0] + branch_2_vector[0], active_paths[path_index][path_last_index][1] + branch_2_vector[1]]
	inactive_paths.append(active_paths[path_index])
	active_paths.append([active_paths[path_index][path_last_index]])
	active_paths.append([active_paths[path_index][path_last_index]])
	if forces:
		branch_1_point = displacement(active_paths, all_points, len(active_paths) - 2, branch_1_point)
		branch_2_point = displacement(active_paths, all_points, len(active_paths) - 1, branch_2_point)
	return [branch_1_point[0], branch_1_point[1], len(active_paths) - 2, branch_2_point[0], branch_2_point[1], len(active_paths) - 1]

# Przedłużenie
def elongate(active_paths, all_points, path_index):
	angle = random.uniform(-a_e, a_e)
	direction = get_simple_direction(active_paths, all_points, path_index)
	path_last_index = len(active_paths[path_index]) - 1
	elongation_vector = [direction[0] * np.cos(angle) - direction[1] * np.sin(angle), direction[0] * np.sin(angle) + direction[1] * np.cos(angle)]
	elongation_point = [active_paths[path_index][path_last_index][0] + elongation_vector[0], active_paths[path_index][path_last_index][1] + elongation_vector[1]]
	if forces:
		elongation_point = displacement(active_paths, all_points, path_index, elongation_point)
	return [elongation_point[0], elongation_point[1], path_index]

# Anihilacja
def annihilation(active_paths, inactive_paths, all_points, path_index):
	points_in_radius = get_points_in_radius(active_paths, all_points, path_index, r_a)
	if len(points_in_radius) == 0:
		return False
	active_points_index = []
	for i in range(0, len(points_in_radius)):
		if check_if_active(points_in_radius[i], active_paths):
			active_points_index.append(i)
	active_points_index.sort(reverse = True)
	for i in active_points_index:
		points_in_radius.pop(i)
	if len(points_in_radius) == 0:
		return False
	closest = get_closest(points_in_radius, active_paths[path_index][len(active_paths[path_index]) - 1])
	active_paths[path_index].append(closest)
	inactive_paths.append(active_paths[path_index])
	return True

# Pierwszy krok czasowy
def first_step(active_paths, inactive_paths, all_points):
	if_branch = random.uniform(0, 1)
	direction = []
	
	if forces:
		if homogeneous:
			direction = [l, 0]
		elif radial:
			direction = [l, 0]
		elif double_radial:
			direction = [l, 0]
		else:
			direction = [l, 0]
	else:
		angle_direction = random.uniform(0, 2 * np.pi)
		direction = [l * np.cos(angle_direction), l * np.sin(angle_direction)]
		
	if if_branch <= b_p:
		angle_1 = random.uniform(a_b_b, a_b_u)
		angle_2 = -random.uniform(a_b_b, a_b_u)
		branch_1 = [direction[0] * np.cos(angle_1) - direction[1] * np.sin(angle_1), direction[0] * np.sin(angle_1) + direction[1] * np.cos(angle_1)]
		branch_2 = [direction[0] * np.cos(angle_2) - direction[1] * np.sin(angle_2), direction[0] * np.sin(angle_2) + direction[1] * np.cos(angle_2)]
		inactive_paths.append(active_paths[0])
		active_paths.append([[0,0], branch_1])
		active_paths.append([[0,0], branch_2])
		active_paths.pop(0)
		insert(all_points, branch_1)
		insert(all_points, branch_2)
	else:
		angle = random.uniform(-a_e, a_e)
		elongation = [direction[0] * np.cos(angle) - direction[1] * np.sin(angle), direction[0] * np.sin(angle) + direction[1] * np.cos(angle)]
		active_paths[0].append(elongation)
		insert(all_points, elongation)

# Krok czasowy
def make_step(active_paths, inactive_paths, all_points):
	paths_to_pop = []
	new_points_elongation = []
	new_points_branch = []
	number_of_active_paths = len(active_paths)
	for i in range(0, number_of_active_paths):
		if_annihilation = annihilation(active_paths, inactive_paths, all_points, i)
		if not if_annihilation:
			if_branch = random.uniform(0, 1)
			if if_branch <= b_p:
				new_points_branch.append(branch(active_paths, all_points, inactive_paths, i))
				paths_to_pop.append(i)
			else:
				new_points_elongation.append(elongate(active_paths, all_points, i))
		else:
			paths_to_pop.append(i)
	for point in new_points_elongation:
		insert(all_points, [point[0], point[1]])
		active_paths[point[2]].append([point[0], point[1]])
	for point in new_points_branch:
		insert(all_points, [point[0], point[1]])
		insert(all_points, [point[3], point[4]])
		active_paths[point[2]].append([point[0], point[1]])
		active_paths[point[5]].append([point[3], point[4]])
	paths_to_pop.sort(reverse = True)
	for x in paths_to_pop:
		active_paths.pop(x)

# Zwraca literę przypisaną do pola zewnetrznego
def field_code():
	if radial:
		return "r"
	elif homogeneous:
		return "h"
	elif double_radial:
		return "dr"
	return ""

# Rysowanie wykresu gałązek
def plot_paths(active_paths, inactive_paths, fpath):
	for path in inactive_paths:
		x = []
		y = []
		for point in path:
			x.append(point[0])
			y.append(point[1])
		#plt.scatter(x, y, c="blue")
		plt.plot(x, y, color="blue")
		
	for path in active_paths:
		x = []
		y = []
		for point in path:
			x.append(point[0])
			y.append(point[1])
		#plt.scatter(x, y, c="blue")
		plt.plot(x, y, color="blue")
		plt.scatter([x[len(x) - 1]], [y[len(y) - 1]], c="red")
	
	plt.axis('scaled')
	s = field_code()
	folder_path = str(fpath) + "T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + s + "_seed_" + str(int(seed))
	os.mkdir(folder_path)
	file_path = folder_path + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + s + "_seed_" + str(int(seed)) + ".png"
	plt.savefig(file_path, format="png")
	plt.clf()

# Rysuje histogram długości gałązek
def plot_paths_lengths(active_paths, inactive_paths, all_lengths, fpath):
	lengths = []
	for path in active_paths:
		length = (len(path) - 1) * l
		lengths.append(length)
		all_lengths.append(length)
	
	for path in inactive_paths:
		if len(path) > 1:
			i1 = len(path) - 1
			i2 = i1 - 1
			last_length = (path[i1][0] - path[i2][0]) * (path[i1][0] - path[i2][0]) + (path[i1][1] - path[i2][1]) * (path[i1][1] - path[i2][1])
			length = (len(path) - 2) * l + last_length
			lengths.append(length)
			all_lengths.append(length)
	
	bins = np.linspace(math.floor(min(lengths)), math.floor(max(lengths)), 20)
	plt.xlim([min(lengths) - l, max(lengths) + l])
	plt.yscale('log')
	plt.hist(lengths, bins=bins, alpha=1)
	plt.xlabel("Długości gałązek")
	plt.ylabel("Liczba gałązek o danej długości")
	s = field_code()
	folder_path = str(fpath) + "T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + s + "_seed_" + str(int(seed))
	file_path = folder_path + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + s + "_seed_" + str(int(seed)) + "_paths_lengths.png"
	plt.savefig(file_path, format="png")
	plt.clf()

# Rysuje wykres gałązek do analizy rozmiaru pól
def plot_paths_for_component_labeling(active_paths, inactive_paths, fpath):
	for path in inactive_paths:
		x = []
		y = []
		for point in path:
			x.append(point[0])
			y.append(point[1])
		plt.plot(x, y, color="black", linewidth=0.5)
		
	for path in active_paths:
		x = []
		y = []
		for point in path:
			x.append(point[0])
			y.append(point[1])
		plt.plot(x, y, color="black", linewidth=0.5)
	
	ax = plt.gca()
	ax.get_xaxis().set_visible(False)
	ax.get_yaxis().set_visible(False)
	plt.box(False)
	s = field_code()
	folder_path = str(fpath) + "T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + s + "_seed_" + str(int(seed))
	file_path = folder_path + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + s + "_seed_" + str(int(seed))
	plt.axis('scaled')
	plt.savefig(file_path + "_component_labeling.png", format="png", dpi=300)
	plt.clf()
	img = Image.open(file_path + "_component_labeling.png").convert('RGB')
	image = np.array(img)
	for i in range(0, image.shape[0]):
		for j in range(0, image.shape[1]):
			sum = int(image[i][j][0]) + int(image[i][j][1]) + int(image[i][j][2])
			if sum < 200:
				image[i][j][0] = 0
				image[i][j][1] = 0
				image[i][j][2] = 0
			else:
				image[i][j][0] = 255
				image[i][j][1] = 255
				image[i][j][2] = 255
	
	sharpend_image = Image.fromarray(image, 'RGB')
	sharpend_image.save(file_path + "_component_labeling_sharpened.png")

# Zwraca czy piksel jest biały
def if_white(arr):
	if arr[0] == 255:
		return True
	return False

# Zwraca do czterech białych sąsiadów w macierzy
def get_neighbours(matrix, visited, y, x):
	neighbours = []
	y_max = matrix.shape[0]
	x_max = matrix.shape[1]
	
	if y + 1 < y_max and visited[y + 1][x] == 0 and if_white(matrix[y + 1][x]):
		neighbours.append([y + 1, x])
		visited[y + 1][x] = 1
	if y - 1 >= 0 and visited[y - 1][x] == 0 and if_white(matrix[y - 1][x]):
		neighbours.append([y - 1, x])
		visited[y - 1][x] = 1
	if x + 1 < x_max and visited[y][x + 1] == 0 and if_white(matrix[y][x + 1]):
		neighbours.append([y, x + 1])
		visited[y][x + 1] = 1
	if x - 1 >= 0 and visited[y][x - 1] == 0 and if_white(matrix[y][x - 1]):
		neighbours.append([y, x - 1])
		visited[y][x - 1] = 1
	
	return neighbours

# Zwraca pole jednego zamkniętego pola
def get_area(image, visited, y, x, pixel_area):
	area = 0
	neighbours = [[y, x]]
	while len(neighbours) != 0:
		area = area + 1
		pixel = neighbours.pop()
		visited[pixel[0]][pixel[1]] = 1
		neig = get_neighbours(image, visited, pixel[0], pixel[1])
		for n in neig:
			neighbours.append(n)
		
	return area * pixel_area

# Zwraca pole jednego piksela
def get_pixel_area(image, all_points):
	points = []
	get_nodes_as_array(all_points, points)
	distance = points[len(points) - 1].val[0] - points[0].val[0]
	left = 0
	right = 0
	y_max = image.shape[0]
	x_max = image.shape[1]
	y = 0
	x = 0
	
	while x < x_max and if_white(image[y][x]):
		while y < y_max and if_white(image[y][x]):
			y = y + 1
		if y >= y_max:
			x = x + 1
			y = 0
	
	left = x
	y = 0
	x = x_max - 1
	
	while x > 0 and if_white(image[y][x]):
		while y < y_max and if_white(image[y][x]):
			y = y + 1
		if y >= y_max:
			x = x - 1
			y = 0
	
	right = x
	#print(left)
	#print(right)
	#print(distance)
	#print((distance / (right - left)) * (distance / (right - left)))
	return (distance / (right - left)) * (distance / (right - left))

# Rysuje histogram rozmiarów obszarów otoczonych przez sieć
def analize_labeled_components(all_areas, all_points, fpath):
	s = field_code()
	folder_path = str(fpath) + "T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + s + "_seed_" + str(int(seed))
	file_path = folder_path + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + s + "_seed_" + str(int(seed))
	img = Image.open(file_path + "_component_labeling_sharpened.png").convert('RGB')
	image = np.array(img)
	visited = np.full((image.shape[0], image.shape[1]), 0)
	sizes = []
	pixel_area = get_pixel_area(image, all_points)
	for i in range(0, image.shape[0]):
		for j in range(0, image.shape[1]):
			if visited[i][j] == 0 and if_white(image[i][j]):
				if i != 0 and j != 0:
					#print(str(i) + ", " + str(j))
					area = get_area(image, visited, i, j, pixel_area)
					sizes.append(area)
					all_areas.append(area)
				else:
					get_area(image, visited, i, j, pixel_area)
			else:
				visited[i][j] = 1
	if len(sizes) > 1:
		bins = np.linspace(min(sizes), max(sizes), 50)
		#plt.xlim([math.floor(min(sizes)) - 10, max(sizes) + 200])
		plt.yscale('log')
		plt.hist(sizes, bins=bins, alpha=1)
		plt.xlabel("Pole obszaru otoczonego gałązkami")
		plt.ylabel("Liczba obszarów o danym polu")
		s = field_code()
		plt.savefig(file_path + "_areas.png", format="png")
		plt.clf()

# Główna funkcja symulująca gałązkowe błądzenie losowe
def barw(all_lengths, all_areas, fpath):
	active_paths = [[[0, 0]]]
	all_points = Node([0, 0])
	inactive_paths = []
	first_step(active_paths, inactive_paths, all_points)
	for i in range(0, T):
		#print(i)
		make_step(active_paths, inactive_paths, all_points)
		all_points = balance_tree(all_points)
	plot_paths(active_paths, inactive_paths, fpath)
	plot_paths_lengths(active_paths, inactive_paths, all_lengths, fpath)
	plot_paths_for_component_labeling(active_paths, inactive_paths, fpath)
	analize_labeled_components(all_areas, all_points, fpath)

# Funkcja wykładnicza z dwoma parametrami
def func(x, a, b):
	return a * np.exp(b * x)

# Wywołanie funkcji symulacji
for j in range(0, 2):
	if j == 0:
		b_p = 0.05
	if j == 1:
		b_p = 0.25
	
	print(f_c)
	all_lengths = []
	all_areas = []
	fpath = "./wyniki_symulacji/T_"
	fpath = fpath + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + field_code()
	os.mkdir(fpath)
	fpath = fpath + "/"
	for i in range(1, 31):
		print("Seed " + str(i))
		seed = i
		random.seed(seed)
		barw(all_lengths, all_areas, fpath)

	if len(all_areas) > 1:
		bins = np.linspace(min(all_areas), max(all_areas), 50)
		#plt.xlim([math.floor(min(all_areas)) - 10, max(all_areas) + 200])
		plt.yscale('log')
		plt.hist(all_areas, bins=bins, alpha=1)
		plt.xlabel("Pole obszaru otoczonego gałązkami")
		plt.ylabel("Liczba obszarów o danym polu")
		plt.title("Parametry sił: $f_c$ = " + str(f_c) + ", $f_e$ = " + str(f_e))
		hist = np.histogram(all_areas, bins=bins)
		values_x = []
		for i in range(0, 49):
			values_x.append((hist[1][i] + hist[1][i + 1]) / 2)
		#plt.ylim([0.9, max(hist[0]) + 50])
		#plt.scatter(values_x, hist[0], color="red", zorder=3)
		#popt, pcov = curve_fit(func, values_x, hist[0], p0=[1.0, -0.1])
	
		s_d = open(str(fpath) + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + field_code() + "_stat_data.txt", "a")
		#s_d.write(str(popt))
		s_d.write("\nŚrednia pól\n")
		s_d.write(str(np.mean(all_areas)))
		s_d.write("\nMediana pól\n")
		s_d.write(str(np.median(all_areas)))
		s_d.write("\n")
		s_d.close()
		#plt.plot(np.linspace(math.floor(min(all_areas)), math.floor(max(all_areas)), 1000), func(np.linspace(math.floor(min(all_areas)), math.floor(max(all_areas)), 1000), popt[0], popt[1]), color="red", zorder=2)
		plt.savefig(str(fpath) + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + field_code() + "_all_areas.png", format="png")
		plt.clf()
		m_a = open(str(fpath) + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + field_code() + "_all_areas.txt", "w+")
		m_a.write(str(all_areas))
		m_a.close()

	bins = np.linspace(math.floor(min(all_lengths)), math.floor(max(all_lengths)), 20)
	plt.xlim([min(all_lengths) - l, max(all_lengths) + l])
	plt.yscale('log')
	plt.hist(all_lengths, bins=bins, zorder=1)
	plt.xlabel("Długości gałązek")
	plt.ylabel("Liczba gałązek o danej długości")
	plt.title("Parametry sił: $f_c$ = " + str(f_c) + ", $f_e$ = " + str(f_e))
	hist = np.histogram(all_lengths, bins=bins)
	values_x = []
	for i in range(0, 19):
		values_x.append((hist[1][i] + hist[1][i + 1]) / 2)
	plt.ylim([0.9, max(hist[0]) + 50])
	popt, pcov = curve_fit(func, values_x, hist[0], p0=[1.0, -0.1])
	s_d = open(str(fpath) + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + field_code() + "_stat_data.txt", "a")
	s_d.write(str(popt))
	s_d.write("\nŚrednia długości\n")
	s_d.write(str(np.mean(all_lengths)))
	s_d.write("\nMediana długości\n")
	s_d.write(str(np.median(all_lengths)))
	s_d.close()
	plt.plot(np.linspace(math.floor(min(all_lengths)), math.floor(max(all_lengths)), 1000), func(np.linspace(math.floor(min(all_lengths)), math.floor(max(all_lengths)), 1000), popt[0], popt[1]), color="red", zorder=2)
	plt.savefig(str(fpath) + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + field_code() + "_all_lengths.png", format="png")
	plt.clf()
	m_l = open(str(fpath) + "/T_" + str(T) + "_b_p_0" + str(int(b_p * 100)) + "_f_c_" + str(int(100 * f_c)) + "_f_e_" + str(int(100 * f_e)) + field_code() + "_all_lengths.txt", "w+")
	m_l.write(str(all_lengths))
	m_l.close()
