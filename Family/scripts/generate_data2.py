#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
"寻根溯源"族谱管理系统 - 数据生成脚本（修正版）
生成要求：
- 至少10个族谱
- 总计不少于100,000条成员数据
- 其中一个族谱拥有50,000以上的成员
- 单个族谱内至少30代人的传承关系
- 每个族谱内的成员至少与另一个成员有亲缘关系
"""

import random
import csv
import os
from datetime import datetime
from collections import defaultdict

random.seed(42)

SURNAMES = [
    '王', '李', '张', '刘', '陈', '杨', '赵', '黄', '周', '吴',
    '徐', '孙', '胡', '朱', '高', '林', '何', '郭', '马', '罗',
    '梁', '宋', '郑', '谢', '韩', '唐', '冯', '于', '董', '萧'
]

MALE_NAMES = [
    '伟', '芳', '娜', '秀', '敏', '静', '丽', '强', '磊', '军',
    '洋', '勇', '艳', '杰', '涛', '明', '超', '秀', '霞', '平',
    '刚', '桂', '英', '华', '建', '云', '海', '雪', '梅', '兰',
    '志', '文', '玉', '荣', '珍', '婷', '玲', '桂', '燕', '旭'
]

FEMALE_NAMES = [
    '芳', '娟', '英', '华', '慧', '巧', '美', '霞', '玲', '燕',
    '萍', '红', '兰', '玉', '珍', '贞', '莉', '兰', '凤', '洁',
    '梅', '琳', '素', '云', '莲', '真', '环', '雪', '荣', '妹',
    '霞', '月', '莺', '媛', '艳', '瑞', '凡', '佳', '嘉', '琼'
]

BIOGRAPHY_TEMPLATES = [
    "一生勤劳朴实，耕读传家。",
    "为人正直，乐于助人，在乡里享有崇高威望。",
    "学识渊博，教书育人，桃李满天下。",
    "精于农事，善于经营，家道日益兴旺。",
    "性情温和，与邻为善，家庭和睦。",
    "历经风雨，坚韧不拔，白手起家。",
    "精通医术，悬壶济世，救人无数。",
    "经商有道，诚信经营，富甲一方。",
    "投笔从戎，保家卫国，战功赫赫。",
    "潜心学问，著述丰富，为后世留下宝贵文化遗产。"
]

OUTPUT_DIR = "e:/DevelopProjects/DatabaseLab/code/Family/database/csv_data"


class FamilyTreeGenerator:
    def __init__(self, genealogy_id, genealogy_name, surname, target_size, min_generations,
                 start_person_id=1, start_family_id=1):
        self.genealogy_id = genealogy_id
        self.genealogy_name = genealogy_name
        self.surname = surname
        self.target_size = target_size
        self.min_generations = min_generations

        # 全局唯一ID起始值，内部计数器从该值开始
        self.person_counter = start_person_id - 1
        self.family_counter = start_family_id - 1

        self.persons = []
        self.families = []
        self.generations_data = defaultdict(list)
        self.birth_year = 1500  # 始祖出生年份

    def generate_name(self, gender):
        if gender == 'M':
            return self.surname + random.choice(MALE_NAMES)
        else:
            return self.surname + random.choice(FEMALE_NAMES)

    def create_person(self, name, gender, birth_year, death_year, generation,
                      birth_family_id=None, biography=None):
        self.person_counter += 1
        person_id = self.person_counter
        if biography is None:
            biography = random.choice(BIOGRAPHY_TEMPLATES)

        person = {
            'person_id': person_id,
            'name': name,
            'gender': gender,
            'birth_year': birth_year,
            'death_year': death_year,
            'biography': biography,
            'generation': generation,
            'genealogy_id': self.genealogy_id,
            'birth_family_id': birth_family_id
        }
        self.persons.append(person)
        self.generations_data[generation].append(person_id)
        return person_id

    def create_family(self, husband_id, wife_id, marriage_year=None):
        self.family_counter += 1
        family_id = self.family_counter
        if marriage_year is None and husband_id:
            husband = next(p for p in self.persons if p['person_id'] == husband_id)
            marriage_year = husband['birth_year'] + random.randint(20, 30)
        family = {
            'family_id': family_id,
            'husband_id': husband_id,
            'wife_id': wife_id,
            'genealogy_id': self.genealogy_id,
            'marriage_year': marriage_year
        }
        self.families.append(family)
        return family_id

    def generate_founder(self):
        """创建始祖，默认男性"""
        name = self.surname + random.choice(MALE_NAMES)
        death_year = self.birth_year + random.randint(60, 90)
        return self.create_person(
            name=name,
            gender='M',
            birth_year=self.birth_year,
            death_year=death_year,
            generation=1
        )

    def generate_large_genealogy(self):
        """生成大型族谱（50000+成员）"""
        founder_id = self.generate_founder()
        print(f"  始祖 {founder_id} 已创建")

        current_gen = 2
        frontier = [founder_id]

        while len(self.persons) < self.target_size and current_gen <= 35:
            new_frontier = []
            for parent_id in frontier:
                if len(self.persons) >= self.target_size:
                    break
                parent = next(p for p in self.persons if p['person_id'] == parent_id)

                # 为每个父辈创建配偶
                spouse_gender = 'F' if parent['gender'] == 'M' else 'M'
                spouse_name = self.generate_name(spouse_gender)
                spouse_birth = parent['birth_year'] + random.randint(-5, 5)
                spouse_death = spouse_birth + random.randint(50, 85) if random.random() > 0.05 else None
                spouse_id = self.create_person(
                    name=spouse_name,
                    gender=spouse_gender,
                    birth_year=spouse_birth,
                    death_year=spouse_death,
                    generation=parent['generation']  # 配偶与父辈同辈
                )

                # 创建家庭
                if parent['gender'] == 'M':
                    husband_id, wife_id = parent_id, spouse_id
                else:
                    husband_id, wife_id = spouse_id, parent_id
                family_id = self.create_family(husband_id=husband_id, wife_id=wife_id)

                # 生子女
                children_count = random.randint(1, 4)
                for _ in range(children_count):
                    if len(self.persons) >= self.target_size:
                        break
                    child_gender = random.choice(['M', 'F'])
                    child_name = self.generate_name(child_gender)
                    child_birth = parent['birth_year'] + random.randint(20, 35)
                    child_death = child_birth + random.randint(50, 85) if random.random() > 0.15 else None
                    child_id = self.create_person(
                        name=child_name,
                        gender=child_gender,
                        birth_year=child_birth,
                        death_year=child_death,
                        generation=current_gen,
                        birth_family_id=family_id  # 直接通过家庭关联
                    )
                    new_frontier.append(child_id)

            frontier = new_frontier
            current_gen += 1
            print(f"  第 {current_gen-1} 代完成，当前 {len(self.persons)} 人")

        print(f"  族谱 {self.genealogy_name} 生成完成，共 {len(self.persons)} 人")

    def generate_normal_genealogy(self):
        """生成普通族谱（同样采用配偶-子女模式）"""
        max_generation = random.randint(self.min_generations, 35)
        founder_id = self.generate_founder()
        print(f"  始祖 {founder_id} 已创建")

        # 生成始祖配偶
        founder = next(p for p in self.persons if p['person_id'] == founder_id)
        spouse_gender = 'F' if founder['gender'] == 'M' else 'M'
        spouse_name = self.generate_name(spouse_gender)
        spouse_birth = founder['birth_year'] + random.randint(-5, 5)
        spouse_death = spouse_birth + random.randint(50, 85) if random.random() > 0.1 else None
        spouse_id = self.create_person(
            name=spouse_name,
            gender=spouse_gender,
            birth_year=spouse_birth,
            death_year=spouse_death,
            generation=1
        )
        if founder['gender'] == 'M':
            family_id = self.create_family(husband_id=founder_id, wife_id=spouse_id)
        else:
            family_id = self.create_family(husband_id=spouse_id, wife_id=founder_id)

        # 使用队列生成后代
        frontier = [(founder_id, family_id, 2)]  # (parent_id, family_id, next_gen)
        while len(self.persons) < self.target_size and frontier:
            new_frontier = []
            for parent_id, fam_id, gen in frontier:
                if len(self.persons) >= self.target_size:
                    break
                parent = next(p for p in self.persons if p['person_id'] == parent_id)

                # 生该代的子女
                children_count = random.randint(1, 5) if gen < 8 else random.randint(1, 3)
                children_ids = []
                for _ in range(children_count):
                    if len(self.persons) >= self.target_size:
                        break
                    child_gender = random.choice(['M', 'F'])
                    child_name = self.generate_name(child_gender)
                    child_birth = parent['birth_year'] + random.randint(18, 35)
                    child_death = child_birth + random.randint(50, 85) if random.random() > 0.15 else None
                    child_id = self.create_person(
                        name=child_name,
                        gender=child_gender,
                        birth_year=child_birth,
                        death_year=child_death,
                        generation=gen,
                        birth_family_id=fam_id
                    )
                    children_ids.append(child_id)

                # 为每个子女创建配偶和家庭，准备下一代
                for child_id in children_ids:
                    if len(self.persons) >= self.target_size:
                        break
                    child = next(p for p in self.persons if p['person_id'] == child_id)
                    # 创建配偶
                    spouse_g = 'F' if child['gender'] == 'M' else 'M'
                    spouse_name = self.generate_name(spouse_g)
                    spouse_birth = child['birth_year'] + random.randint(-5, 5)
                    spouse_death = spouse_birth + random.randint(50, 85) if random.random() > 0.1 else None
                    sp_id = self.create_person(
                        name=spouse_name,
                        gender=spouse_g,
                        birth_year=spouse_birth,
                        death_year=spouse_death,
                        generation=gen
                    )
                    if child['gender'] == 'M':
                        new_fam = self.create_family(husband_id=child_id, wife_id=sp_id)
                    else:
                        new_fam = self.create_family(husband_id=sp_id, wife_id=child_id)
                    # 只有代数未达上限才加入下一代
                    if gen + 1 <= max_generation:
                        new_frontier.append((child_id, new_fam, gen + 1))

            frontier = new_frontier
            print(f"  当前 {len(self.persons)} 人 (第 {gen} 代)")

        # 若还不够人数，随机扩展分支
        while len(self.persons) < self.target_size:
            # 选择一个已有后代（避免始祖）
            ext_gen = random.randint(2, min(max_generation, 5))
            if self.generations_data[ext_gen]:
                extend_person_id = random.choice(self.generations_data[ext_gen])
                ext_person = next(p for p in self.persons if p['person_id'] == extend_person_id)
                # 为该人创建配偶
                spouse_g = 'F' if ext_person['gender'] == 'M' else 'M'
                sname = self.generate_name(spouse_g)
                sbirth = ext_person['birth_year'] + random.randint(-5, 5)
                sdeath = sbirth + random.randint(50, 85) if random.random() > 0.1 else None
                sp_id = self.create_person(
                    name=sname,
                    gender=spouse_g,
                    birth_year=sbirth,
                    death_year=sdeath,
                    generation=ext_person['generation']
                )
                if ext_person['gender'] == 'M':
                    fam = self.create_family(husband_id=extend_person_id, wife_id=sp_id)
                else:
                    fam = self.create_family(husband_id=sp_id, wife_id=extend_person_id)
                # 添加子女
                for _ in range(random.randint(1, 4)):
                    if len(self.persons) >= self.target_size:
                        break
                    cgender = random.choice(['M', 'F'])
                    cname = self.generate_name(cgender)
                    cbirth = ext_person['birth_year'] + random.randint(20, 35)
                    cdeath = cbirth + random.randint(50, 85) if random.random() > 0.15 else None
                    self.create_person(
                        name=cname,
                        gender=cgender,
                        birth_year=cbirth,
                        death_year=cdeath,
                        generation=ext_person['generation'] + 1,
                        birth_family_id=fam
                    )

        print(f"  族谱 {self.genealogy_name} 生成完成，共 {len(self.persons)} 人")

    def generate(self):
        if self.target_size >= 40000:
            self.generate_large_genealogy()
        else:
            self.generate_normal_genealogy()
        return self.persons, self.families


class DataExporter:
    def __init__(self, output_dir):
        self.output_dir = output_dir
        os.makedirs(output_dir, exist_ok=True)

    def export_dicts_to_csv(self, filename, data, fieldnames):
        filepath = os.path.join(self.output_dir, filename)
        with open(filepath, 'w', newline='', encoding='utf-8') as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames, extrasaction='ignore')
            writer.writeheader()
            writer.writerows(data)
        print(f"  导出: {filename} ({len(data)} 条记录)")

    def export_all(self, users, genealogies, user_genealogy, persons, families):
        # 用户表
        self.export_dicts_to_csv('users.csv', users, 
                                 ['user_id', 'username', 'password_hash', 'email', 'created_at'])
        # 族谱表
        self.export_dicts_to_csv('genealogies.csv', genealogies,
                                 ['genealogy_id', 'name', 'surname', 'compile_time', 'description',
                                  'creator_id', 'created_at'])
        # 用户-族谱协作表
        self.export_dicts_to_csv('user_genealogy.csv', user_genealogy,
                                 ['user_id', 'genealogy_id', 'role', 'joined_at'])
        # 成员表
        self.export_dicts_to_csv('persons.csv', persons,
                                 ['person_id', 'name', 'gender', 'birth_year', 'death_year',
                                  'biography', 'generation', 'genealogy_id', 'birth_family_id'])
        # 家庭表
        self.export_dicts_to_csv('families.csv', families,
                                 ['family_id', 'husband_id', 'wife_id', 'genealogy_id', 'marriage_year'])


def generate_users(count=20):
    users = []
    for i in range(1, count + 1):
        users.append({
            'user_id': i,
            'username': f'user{i}',
            'password_hash': f'hash_{i}',
            'email': f'user{i}@example.com',
            'created_at': datetime.now().replace(microsecond=0).isoformat()
        })
    return users


def main():
    print("=" * 60)
    print("族谱管理系统 - 模拟数据生成器（修正版）")
    print("=" * 60)

    # 族谱配置 (满足要求：一个≥50000，总共≥100000，至少30代传承)
    genealogy_configs = [
        {'id': 1, 'name': '李氏大宗谱', 'surname': '李', 'size': 55000, 'min_generations': 30, 'creator_id': 1, 'editors': [2, 3]},
        {'id': 2, 'name': '王氏族谱', 'surname': '王', 'size': 8000, 'min_generations': 20, 'creator_id': 2, 'editors': []},
        {'id': 3, 'name': '张氏族谱', 'surname': '张', 'size': 7500, 'min_generations': 18, 'creator_id': 3, 'editors': [1]},
        {'id': 4, 'name': '刘氏族谱', 'surname': '刘', 'size': 6000, 'min_generations': 15, 'creator_id': 4, 'editors': []},
        {'id': 5, 'name': '陈氏族谱', 'surname': '陈', 'size': 5500, 'min_generations': 14, 'creator_id': 5, 'editors': [2]},
        {'id': 6, 'name': '杨氏族谱', 'surname': '杨', 'size': 5000, 'min_generations': 13, 'creator_id': 6, 'editors': []},
        {'id': 7, 'name': '赵氏族谱', 'surname': '赵', 'size': 4500, 'min_generations': 12, 'creator_id': 7, 'editors': [1, 3]},
        {'id': 8, 'name': '黄氏族谱', 'surname': '黄', 'size': 4000, 'min_generations': 11, 'creator_id': 8, 'editors': []},
        {'id': 9, 'name': '周氏族谱', 'surname': '周', 'size': 3500, 'min_generations': 10, 'creator_id': 9, 'editors': [5]},
        {'id': 10, 'name': '吴氏族谱', 'surname': '吴', 'size': 3000, 'min_generations': 10, 'creator_id': 10, 'editors': []},
        {'id': 11, 'name': '徐氏族谱', 'surname': '徐', 'size': 2800, 'min_generations': 9, 'creator_id': 1, 'editors': [4]},
        {'id': 12, 'name': '孙氏族谱', 'surname': '孙', 'size': 2500, 'min_generations': 8, 'creator_id': 2, 'editors': []},
    ]

    total_target = sum(c['size'] for c in genealogy_configs)
    print(f"\n计划生成 {len(genealogy_configs)} 个族谱，共约 {total_target:,} 人")

    # 生成用户
    users = generate_users(20)
    print(f"\n生成用户数据: {len(users)} 个用户")

    # 准备收集全局数据
    genealogies = []
    user_genealogy = []
    all_persons = []
    all_families = []

    global_person_offset = 1
    global_family_offset = 1

    for idx, config in enumerate(genealogy_configs):
        print(f"\n正在生成族谱 {idx+1}/{len(genealogy_configs)}: {config['name']}")

        # 创建生成器，传入全局起始ID
        gen = FamilyTreeGenerator(
            genealogy_id=config['id'],
            genealogy_name=config['name'],
            surname=config['surname'],
            target_size=config['size'],
            min_generations=config['min_generations'],
            start_person_id=global_person_offset,
            start_family_id=global_family_offset
        )

        persons, families = gen.generate()
        all_persons.extend(persons)
        all_families.extend(families)

        # 更新全局偏移量
        global_person_offset += len(persons)
        global_family_offset += len(families)

        # 族谱信息
        genealogies.append({
            'genealogy_id': config['id'],
            'name': config['name'],
            'surname': config['surname'],
            'compile_time': f'{random.randint(1990, 2024)}-{random.randint(1, 12):02d}-{random.randint(1, 28):02d}',
            'description': f'{config["surname"]}氏族谱，详细记录了家族历代传承。',
            'creator_id': config['creator_id'],
            'created_at': datetime.now().replace(microsecond=0).isoformat()
        })

        # 用户-族谱关系
        user_genealogy.append({
            'user_id': config['creator_id'],
            'genealogy_id': config['id'],
            'role': 'creator',
            'joined_at': datetime.now().replace(microsecond=0).isoformat()
        })
        for editor_id in config.get('editors', []):
            user_genealogy.append({
                'user_id': editor_id,
                'genealogy_id': config['id'],
                'role': 'editor',
                'joined_at': datetime.now().replace(microsecond=0).isoformat()
            })

    print(f"\n数据生成汇总:")
    print(f"  族谱: {len(genealogies)} 个")
    print(f"  成员: {len(all_persons)} 人")
    print(f"  家庭: {len(all_families)} 个")
    print(f"  用户-族谱关系: {len(user_genealogy)} 条")

    # 导出CSV
    exporter = DataExporter(OUTPUT_DIR)
    print(f"\n导出CSV文件到: {OUTPUT_DIR}")
    exporter.export_all(users, genealogies, user_genealogy, all_persons, all_families)

    print("\n" + "=" * 60)
    print("数据生成完成！")
    print("=" * 60)


if __name__ == '__main__':
    main()