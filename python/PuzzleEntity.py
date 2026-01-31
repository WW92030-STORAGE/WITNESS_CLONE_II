from enum import Enum

class Color(Enum):
    NIL = (-1, -1, -1)
    
    RGB_RED = (255, 0, 0)
    RGB_GREEN = (0, 255, 0)
    RGB_BLUE = (0, 0, 255)
    RGB_YELLOW = (255, 255, 0)
    RGB_CYAN = (0, 255, 255)
    RGB_MAGENTA = (255, 0, 255)
    RGB_BLACK = (0, 0, 0)
    RGB_WHITE = (255, 255, 255)
    
    RGB_ORANGE = (255, 128, 0)
    RGB_GREY = (128, 128, 128)
    RGB_DARK = (64, 64, 64)
    RGB_LIGHT = (192, 192, 192)

    # Furries!

    SP2 = 0xCEB7FF,
    SP1 = 0x8AD8FF

COLORS = [Color.RGB_RED, Color.RGB_YELLOW, Color.RGB_GREEN, Color.RGB_CYAN, Color.RGB_BLUE, Color.RGB_MAGENTA, Color.RGB_WHITE]

class PuzzleEntity:
    color = Color.NIL
    isPath = False
    hasLine = 0
    isBlocker = False
    type = "PuzzleEntity"
    disp = "OBJECT"

    isActive = True

    def init(self):
        pass

    def __init__(self):
        self.init()
    
    def clear(self):
        return PuzzleEntity()

    def getregs(self):
        return [self.isPath, self.hasLine, self.isBlocker]

    def setregs(self, v):
        self.isPath = v[0]
        if len(v) > 1:
            self.hasLine = v[1]
        if len(v) > 2:
            self.isBlocker = v[2]
        

class Endpoint(PuzzleEntity):
    isStart = True

    def init(self):
        self.type = "Endpoint"
        self.disp = "ENDPT_"
        if self.isStart:
            self.disp = "START_"
    
    def __init__(self, s = True):
        super().__init__()
        self.isPath = True
        self.isStart = s
        self.init()
    

class PathDot(PuzzleEntity):
    restriction = 0

    def init(self):
        self.type = "PathDot"
        self.disp = "PDOT" + str(self.restriction)
        while len(self.disp) < 6:
            self.disp = self.disp + "_"
        
    def __init__(self, res = 0):
        super().__init__()
        self.isPath = True
        self.restriction = res
        self.init()
    
class ColorEntity(PuzzleEntity):
    def init(self):
        self.type = "ColoredEntity"
        self.disp = "COLORX"
    
    def __init__(self, c = Color.RGB_BLACK):
        super().__init__()
        self.color = c
        self.init()
    
class Blob(ColorEntity):
    def init(self):
        self.type = "Blob"
        self.disp = " BLOB "
        
    def __init__(self, c = Color.RGB_BLACK):
        super().__init__(c)

class Star(ColorEntity):
    def init(self):
        self.type = "Star"
        self.disp = " STAR "
        
    def __init__(self, c = Color.RGB_BLACK):
        super().__init__(c)

class Triangle(ColorEntity):
    count = 1
    def init(self):
        self.type = "Triangle"
        self.disp = " TRIX" + str(self.count)
        
    def __init__(self, x = 1, c = Color.RGB_ORANGE):
        self.count = x
        super().__init__(c)
    
class Cancel(ColorEntity):
    def init(self):
        self.type = "Cancel"
        self.disp = "CANCEL"
        
    def __init__(self, c = Color.RGB_WHITE):
        super().__init__(c)
